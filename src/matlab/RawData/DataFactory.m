classdef DataFactory < handle
    % Cria dados de entrada falsos (aceleração, giro, e magnetômetro)
    % tendo como base um arquivo de entrada, seguindo o formato de exemplo em dados/fake/exemplo.txt
    % O input consiste em dados de posição e inclinação, tem como saída dados de acel, giro e mag
    % adicionados de ruído branco.
    % 
    % - O construtor recebe parâmetros de criação, como o arquivo de entrada
    % - A função generate, cria um arquivo com os dados fake, seguindo os padrões de formatação do arquivo global
    %   e retorna o endereço relativo a pasta fake, do arquivo
    % - A função read, retorna uma linha do dado fake interpolado
    %   isto é, aceleração e inclinação para 1 amostra (após interpolação) 
    %   Utilizando a função read novamente ela vai ler a linha seguinte até o fim do arquivo.

    properties
        generated_path
        source_path

        add_noise_bias
        interpolated_data
        absolute_gyro
        read_ptr

        % Parametros para simular o MPU
        gravity_value
        esc_ac
        esc_giro
        esc_mag
        sample_freq
        sample_period % em ms

        noise_radio

        % Por motivos de debug
        debug_on
        pos, vel, acel, ang, gyro, mag
    end

    methods (Access=public)
        function obj = DataFactory(file_name, sample_freq, esc_ac, esc_giro, debug_on)
            % Nome do arquivo na pasta 'dados/fake/' com extensão
            obj.source_path = file_name;
            obj.generated_path = strcat(file_name, '.generated');
            obj.sample_freq = sample_freq;
            obj.sample_period = 1000/sample_freq; % em ms
            obj.gravity_value = 9.8;
            obj.esc_ac = esc_ac;
            obj.esc_giro = esc_giro;
            obj.esc_mag = 4912;
            obj.debug_on = debug_on;
            obj.noise_radio = 25; % quanto maior menor o ruído
            obj.add_noise_bias = true;
        end

        function out_file_name = generate(obj)
            % Interpola os dados base, gera dados, salva em um arquivo e retorna o nome do arquivo
            fprintf('Gerando dados a partir de "%s"...\n', obj.source_path);
            
            fprintf('Interpolando...\n');
            obj.interpolated_data = obj.interpolate();
            
            % Os angulos interpolados são relativos, precisamos deles em absoluto,
            % sendo descritos em zyx com relação a posição inicial, que deve ser 0.
            obj.absolute_gyro = obj.convert_ang(obj.interpolated_data(:, 5:7));

            generated_f_ptr = obj.open_file(obj.generated_path, 'w');
            fprintf(generated_f_ptr, '#[m\n');

            fprintf('Escrevendo metadados...\n');
            obj.write_metadatas(generated_f_ptr);

            fprintf('Gerando acel...\n');
            acel = obj.generate_acel();

            fprintf('Gerando gyro...\n');
            gyro = obj.generate_gyro();

            fprintf('Gerando mag...\n');
            mag = obj.generate_mag();

            fprintf('Salvando dados...\n');
            count = 0;
            fake_address = 0;
            for i=1:size(obj.interpolated_data,1)
                obj.write_line(generated_f_ptr, count, fake_address, acel(i,:), gyro(i,:), mag(i,:));
                count = count+1;
            end

            fprintf(generated_f_ptr, 'm]#\n');
            fclose(generated_f_ptr);

            % Para fins de debug, plota todos os dados
            if obj.debug_on
                obj.plot_all();
            end

            out_file_name = obj.generated_path;
            fprintf('Dados gerados no arquivo "%s".\n', out_file_name);
        end

        function sample = read(obj)
            % Lê o baseline (dado interpolado)
            % retorna um array contendo posição x,y,z em metros e inclinação zyx em graus
            % OBS: a inclinação retornada é a absoluta (em ângulos de Euler)
            if length(obj.read_ptr) < 1
                obj.read_ptr = 1;
            elseif obj.read_ptr > size(obj.interpolated_data,1)
                sample = [];
                return
            end

            d = [obj.interpolated_data(obj.read_ptr, 1:4), obj.absolute_gyro(obj.read_ptr, :)];
            obj.read_ptr = obj.read_ptr + 1;

            % converte posição em cm p/ metros
            sample = d ./ [1, 100, 100, 100, 1, 1, 1];
        end
    end

    methods (Access=private)
        function f_ptr = open_file(obj, path, mode)
            [f_ptr, err] = fopen(path, mode, 'n', 'UTF-8');
            
            if (f_ptr==-1)
                error('Nao abriu arquivo de nome: %s.\n', path);
                return
            end
        end

        function line_data = read_line(obj, f_ptr)
            % Lê uma linha de dado, isso é, não vazia e não é comentário
            is_empty_line = true;

            while is_empty_line && ~feof(f_ptr)
                % Le ate achar quebra de linha, e remove \r \n e espaços do inicio e final da string
                l = fgetl(f_ptr);
                l = strtrim(l);

                if length(l) > 1 && ~strcmp(l(1), '#')
                    is_empty_line = false;
                end
            end
            
            if is_empty_line
                line_data = '';
            else
                line_data = l;
            end
        end

        function ret = cut_on_esc(obj, data_arr, scale)
            ret = zeros(1, length(data_arr));

            for i = 1:length(data_arr)
                if abs(data_arr(i)) > scale
                    ret(i) = sign(data_arr(i)) * scale;
                else
                    ret(i) = data_arr(i);
                end
            end
        end

        function write_line(obj, f_ptr, count, address, acel, gyro, mag)
            % Escreve um vetor de array de unit16 em um arquivo, separado por espaço
            
            acel = obj.cut_on_esc(acel, obj.esc_ac);
            gyro = obj.cut_on_esc(gyro, obj.esc_giro);
            mag = obj.cut_on_esc(mag, obj.esc_mag);

            acel = 32767 * acel / obj.esc_ac;
            gyro = 32767 * gyro / obj.esc_giro;
            mag = 32767 * mag / obj.esc_mag;
            data_arr = [count, address, acel, gyro, mag];

            for i = 1:length(data_arr)
                fprintf(f_ptr, '%d ', typecast(int16(data_arr(i)), 'uint16'));
            end
            fprintf(f_ptr, '\n');
        end

        function data = parse_line(obj, line)
            % Transforma a string do dado em números, retornando na mesma ordem em um array
            data=strsplit(line, ' ');
            if ~strcmp(cell2mat(data(1)), '')
                data = cellfun(@str2num, data);
            else
                data = [];
            end
        end 

        function data = read_all_source(obj)
            source_f_ptr = obj.open_file(obj.source_path, 'r');
            
            data = [];
            while ~feof(source_f_ptr)
                l = obj.read_line(source_f_ptr);
                d = obj.parse_line(l);

                if length(d) < 1
                    break;
                elseif length(d) ~= 7
                    error('Linha contém quantidade incorreta de dados, era esperado 7, tem %d', length(d));
                end

                data = vertcat(data, d);
            end

            fclose(source_f_ptr);
        end

        function ret = interpolate(obj)
            % Interpola amostras
            data = obj.read_all_source();
            old_x = data(:,1);
            new_x = old_x(1):obj.sample_period:old_x(length(old_x));
            data(:, 5:7) = cumsum(data(:,5:7));
            ret = interp1(old_x, data, new_x, 'pchip');
        end

        function ret = convert_ang(obj, ang_data)
            ret = zeros(size(ang_data,1), 3);
            abs_rot = ang2rotZYX(ang_data(1,1), ang_data(1,2), ang_data(1,3));
            
            for i = 2:size(ang_data,1)
                old_d = ang_data(i-1, :);
                d = ang_data(i, :);

                delta = d - old_d;
                rel_rot = ang2rotZYX(delta(1), delta(2), delta(3));

                abs_rot = abs_rot * rel_rot;
                ret(i, :) = rot2angZYX(abs_rot);
            end
        end

        function write_metadatas(obj, f_ptr)
            % Escreve no arquivo global os metadados
            fprintf(f_ptr, '041020\n');
            fprintf(f_ptr, '145330\n');
            fprintf(f_ptr, '6525\n');
            fprintf(f_ptr, '49878\n');
            fprintf(f_ptr, '233088\n');
            fprintf(f_ptr, '1 1 1 1 1 0 0 0 0 0 0\n');
            fprintf(f_ptr, '1 1 1 1 1 0 0 0 1 1 1\n');
            fprintf(f_ptr, '20046 21331 %d %d %d 5 1  2  3  4  5\n', obj.esc_ac, obj.esc_giro, obj.sample_freq);
            fprintf(f_ptr, '%d\n', size(obj.interpolated_data, 1));
        end
        
        function ret = generate_acel(obj)
            % Usando amostras de dados de entrada (posição em cm e inclinação absoluta em graus),
            % gera uma linha de dado do acelerômetro
            d = obj.interpolated_data;

            % Obtem aceleração pela derivada numerica, adiantada f'(i) = (f(i+1) - f(i)) / 2
            % Devido a derivada numerica, acel e vel não tem valor p/ a ultima amostra
            % joguei aqui como sendo o valor anterior
            % OBS: a integral numérica por trapésio, não retorna o dado original, anulando a derivada.
            % Nenhuma das outras também retorna, somente funciona a integral atrasada f(i) = f'(i-1) * t
            pos = d(:, 2:4);
            vel = diff(pos) / obj.sample_period;
            vel = vertcat(vel, vel(size(vel,1),:));
            acel = diff(vel) / obj.sample_period;
            acel = vertcat(acel, acel(size(acel,1),:));

            % converte cm/ms^2 p/ g
            acel = acel * 1000000 / (100 * obj.gravity_value);

            % adiciona o vetor da gravidade
            vector = zeros(size(acel,1), 3);
            vector(:, 3) = 1;
            acel = acel + vector;

            % Rotaciona a aceleração que era global, para fingir que foram lidas pelo sensor
            % Por mais estranho que pareçaa, a rotação deve ser na inversa 
            for i = 1:size(obj.interpolated_data,1)
                d = obj.absolute_gyro(i, :);
                rot = ang2rotZYX(d(1), d(2), d(3));
                rot_inv = rot'; % transpor matriz de rotação é igual a inverter
            
                data = rot_inv * acel(i, :)';
                acel(i, :) = data';
            end

            % adiciona ruído branco gaussiano e bias
            % Valores arbitrários (só não exagerar, este bias simula o bias após correção)
            % seria o resto que não deu para ser corrigido com a calibração
            if obj.add_noise_bias
                acel(:,1) = acel(:,1) + 0.0037;
                acel(:,2) = acel(:,2) - 0.023;
                acel(:,3) = acel(:,3) + 0.025;
                acel = awgn(acel, obj.noise_radio, 'measured');
            end

            % Para debug
            if obj.debug_on
                obj.pos = pos;
                obj.vel = vel;
                obj.acel = acel;
            end

            % insere ruído
            
            ret = acel;
        end
        
        function ret = generate_gyro(obj)
            % Usando amostras de dados de entrada (posição em cm e inclinação relativa em graus),
            % gera uma linha de dado do giroscópio
            
            relative_gyro = obj.interpolated_data(:, 5:7);

            % Deriva as posições relativas para obter a velociade angular
            % Devido a derivada numerica, gyro não tem valor p/ a ultima amostra
            % joguei aqui como sendo o valor anterior
            % OBS: a integral numérica por trapésio, não retorna o dado original, anulando a derivada.
            % Nenhuma das outras também retorna, somente funciona a integral atrasada f(i) = f'(i-1) * t
            gyro = diff(relative_gyro) / obj.sample_period;
            gyro = vertcat(gyro, gyro(size(gyro,1),:));

            % converte giro/ms para giro/s
            gyro = gyro*1000;

            % adiciona ruído branco gaussiano e bias
            % Valores arbitrários (só não exagerar)
            if obj.add_noise_bias
                gyro(:,1) = gyro(:,1) + 0.037;
                gyro(:,2) = gyro(:,2) - 0.082;
                gyro(:,3) = gyro(:,3) + 0.065;
                gyro = awgn(gyro, obj.noise_radio, 'measured');
            end

            % Para debug
            if obj.debug_on
                obj.ang = obj.absolute_gyro;
                obj.gyro = gyro;
            end

            % Insere ruído
            ret = gyro;
        end
        
        function ret = generate_mag(obj)
            % Usando amostras de dados de entrada (posição em cm e inclinação absoluta em graus),
            % gera uma linha de dado do magnetômetro
            mag = zeros(size(obj.interpolated_data,1), 3);

            for i = 1:size(obj.interpolated_data,1)
                d = obj.absolute_gyro(i, :);
                rot = ang2rotZYX(d(1), d(2), d(3));
                rot_inv = rot'; % transpor matriz de rotação é igual a inverter
            
                % Cria um dado falso de magnetômetro, o array abaixo segue o modelo:
                % https://www.mikrocontroller.net/attachment/292888/AN4248.pdf
                % e é arbitrário, poderia ser qualquer valor de ângulo,
                % Com a diferença que Z aponta para cima, Y para a esquerda e X para frente
                % (que representa a incidência do campo magnético nesta região do planeta)
                % É arbitrário porque para estimar a inclinação esse dado acaba sendo anulado 
                data = rot_inv * [25*cosd(2); 0; -25*sind(2)];
                
                mag(i,:) = data';
            end

            % adiciona ruído branco gaussiano e bias
            % Valores arbitrários (só não exagerar)
            if obj.add_noise_bias
                mag(:,1) = (mag(:,1) + 2.6) * 1.00208;
                mag(:,2) = (mag(:,2) - 1.21) * 0.98213;
                mag(:,3) = (mag(:,3) + 3.55) * 1.00423;
                mag = awgn(mag, obj.noise_radio, 'measured');
            end
            
            % Para debug. Usa versão com os eixos ainda não rotacionados
            % porque o dashboard vai exibir depois de des-rotacionar
            % assim dá p/ comparar visualmente os dados
            if obj.debug_on
                obj.mag = mag;
            end

            % inverte os eixos, para corresponder ao mesmo da placa do MPU9250
            ret = zeros(size(obj.interpolated_data,1), 3);
            ret(:,1) = mag(:, 2);
            ret(:,2) = mag(:, 1);
            ret(:,3) = -mag(:, 3);

            % insere ruído
        end

        function plot_all(obj)
            % Usado apenas para plotar os dados, para fins de debug
            % Como por exemplo ver como ficou a interpolação

            figure('Name','DataFactory - Debug');
            s_title = {'pos (cm X ms)', 'vel (cm/ms X ms)', 'acel (g X ms)', 'ang (º X ms)', 'gyro (º/s X ms)', 'mag (uT X ms)'};
            sources = {obj.pos, obj.vel, obj.acel, obj.ang, obj.gyro, obj.mag};
            x_axis = obj.interpolated_data(:,1);
            style = {'-r', '-g', '-b'};
            count=1;
            for i = 1:2
                for j = 1:3
                    subplot(2, 3, count);
                    hold on
                    for k = 1:3
                        plot(x_axis, sources{count}(:,k), style{k});
                    end
                    hold off
                    title(s_title{count});
                    legend({'x', 'y', 'z'});
                    count = count+1;
                end
            end
        end
    end
end