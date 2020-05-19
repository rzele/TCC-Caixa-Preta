classdef reader < handle
    %READER Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        read_from_serial
        f_pt
        serial_com
        serial_baudrate
        file_full_path
        file_simulated_freq
        stop_wait = false
        read_attempts = 5
        read_current_attempts = 0
        time
    end
    
    methods
        function obj = reader()

            % Se a porta serial ja estiver aberta tenta fechar
            try
                fclose(instrfind);
            catch ME
            end    
        end

        function set_serial_reader(obj, serial_com, serial_baudrate)
            obj.read_from_serial = true;
            obj.serial_com = serial_com;
            obj.serial_baudrate = serial_baudrate;
            obj.file_simulated_freq = Inf;

            obj.f_pt = serial(serial_com, 'Baudrate', serial_baudrate);
            fopen(obj.f_pt);

            if (obj.f_pt==-1)
                fprintf(1,'Nao abriu COM3.\n');
                return
            end
            fprintf('Pronto para receber dados!\n');
            fprintf('Por favor, selecione Teste 12 na Caixa Preta.\n');

            obj.wait_start_signal();
        end

        function set_file_reader(obj, file_full_path, file_simulated_freq)
            obj.read_from_serial = false;
            obj.file_full_path = file_full_path;
            obj.file_simulated_freq = file_simulated_freq;
            obj.time = now;

            [obj.f_pt, err] = fopen(file_full_path, 'r', 'n', 'UTF-8');

            if (obj.f_pt==-1)
                fprintf(1,'Nao abriu arquivo de nome: %s.\n', file_full_path);
                fprintf(1,'%s.\n', err);
                return
            end
            fprintf('Buscando sinal de start\n');

            obj.wait_start_signal();
        end

        function wait_start_signal(obj)
            while ~obj.stop_wait
                temp = fgetl(obj.f_pt);
                if strcmp(temp,'start') == 1
                    obj.stop_wait = true;
                    break;
                end
                fprintf('%s\n', temp);
            end
            fprintf('Iniciando leitura.\n');
        end

        function force_stop_wait(obj)
            obj.stop_wait = true;
        end

        function data = read_sample(obj)
            % Simula um delay de leitura, para simular leitura em tempo real
            while (now-obj.time)*100000 < 1/obj.file_simulated_freq
            end
            obj.time = now;

            % Lê até achar quebra de linha
            data=fgetl(obj.f_pt);
            
            % Checa se é o final da transmissão
            if ~isempty(strfind(data,'fim'))
                data = [];
                return;
            end

            % Quebra a string no marcador ';'
            data=strsplit(data,';');

            % Checa se realmente leu todos os dados, se n lê novamente até no maximo 'read_attempts' vezes
            if length(data) < 7
                fprintf('Erro ao ler dados, tentando novamente - %d\n', obj.read_current_attempts);
                if (obj.read_current_attempts == obj.read_attempts)
                    data = [];
                    return;
                end
                obj.read_current_attempts = obj.read_current_attempts + 1;
                data = obj.read_sample();
                obj.read_current_attempts = 0;
            end
        end

        % Fecha o arquivo ou conexão com porta serial
        function delete(obj)
            fclose(obj.f_pt);
        end
    end
end

