classdef ReaderCore < handle
    %READERCORE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        f_pt
        read_attempts = 5
        read_current_attempts = 0
        start_delimiter 
        end_delimiter 
        data_delimiter 
        metadatas = struct()
        count_reader_times = 0
        time_reading = 0;
    end
    
    methods
        function obj = ReaderCore(start_delimiter, end_delimiter, data_delimiter)
        end

        function wait_start_signal(obj)
            fprintf('Aguardando sinal de start\n')
            tic
            while toc < 4
                fprintf('.')
                temp = obj.read_not_empty_line();
                if strcmp(temp, obj.start_delimiter) == 1
                    fprintf('\nIniciando leitura.\n\n');
                    return
                end
            end
            error('Sinal demorou mais do que 4 segundos p/ ser enviado.');
        end

        function set_metadatas(obj)
            obj.metadatas.('colision_date') = obj.read_not_empty_line();
            obj.metadatas.('colision_time') = obj.read_not_empty_line();
            obj.metadatas.('colision_temperature') = str2num(obj.read_not_empty_line());
            obj.metadatas.('SRAM_MPU_adr') = str2num(obj.read_not_empty_line());
            obj.metadatas.('SRAM_GPS_adr') = str2num(obj.read_not_empty_line());
            
            l1 = strsplit(obj.read_not_empty_line(), ' ');
            l2 = strsplit(obj.read_not_empty_line(), ' ');
            l3 = strsplit(obj.read_not_empty_line(), ' ');
            
            obj.metadatas.('n_samples') = str2num(obj.read_not_empty_line());

            obj.metadatas.('cf_ok') = l1(1);
            obj.metadatas.('st_cf') = l1(2);
            obj.metadatas.('aesc_cf') = l1(3);
            obj.metadatas.('gesc_cf') = l1(4);
            obj.metadatas.('tp_cf') = l1(5);
            obj.metadatas.('ax_offset') = str2int16(l1(6));
            obj.metadatas.('ay_offset') = str2int16(l1(7));
            obj.metadatas.('az_offset') = str2int16(l1(8));
            obj.metadatas.('gx_offset') = str2int16(l1(9));
            obj.metadatas.('gy_offset') = str2int16(l1(10));
            obj.metadatas.('gz_offset') = str2int16(l1(11));

            obj.metadatas.('cf_ok_mag') = l2(1);
            obj.metadatas.('sth_cf_mag') = l2(2);
            obj.metadatas.('hx_ASA') = str2int16(l2(3));
            obj.metadatas.('hy_ASA') = str2int16(l2(4));
            obj.metadatas.('hz_ASA') = str2int16(l2(5));
            obj.metadatas.('hx_offset') = str2int16(l2(6));
            obj.metadatas.('hy_offset') = str2int16(l2(7));
            obj.metadatas.('hz_offset') = str2int16(l2(8));
            obj.metadatas.('hx_escala') = str2int16(l2(9));
            obj.metadatas.('hy_escala') = str2int16(l2(10));
            obj.metadatas.('hz_escala') = str2int16(l2(11));

            obj.metadatas.('st_op') = l3(1);
            obj.metadatas.('sth_op') = l3(2);
            obj.metadatas.('aesc_op') = str2int16(l3(3));
            obj.metadatas.('gesc_op') = str2int16(l3(4));
            obj.metadatas.('fammost') = str2int16(l3(5));
            obj.metadatas.('bw_ag') = l3(6);
        end

        function line_str = read_not_empty_line(obj)
            is_empty_line = true;

            while is_empty_line
                % Le ate achar quebra de linha, e remove \r \n e espa�os do inicio e final da string
                line_str = fgetl(obj.f_pt);
                line_str = strtrim(line_str);

                if length(line_str) > 1
                    is_empty_line = false;
                end
            end
        end

        function data = read_sample(obj)
            obj.count_reader_times = obj.count_reader_times + 1;
            t1 = tic;
            % Le ate achar quebra de linha
            data=fgetl(obj.f_pt);
           
            % Checa se e o final da transmissao
            if ~isempty(strfind(data, obj.end_delimiter))
                data = [];
                return;
            end

            % Quebra a string no marcador
            data=strsplit(data, obj.data_delimiter);

            % Checa se realmente leu todos os dados, se n le novamente ate no maximo 'read_attempts' vezes
            if length(data) < 11
                if (obj.read_current_attempts == obj.read_attempts)
                    data = [];
                    error('Error at read data, was not possible to read some datas. check if the file have the correct format')
                    return;
                end
                obj.read_current_attempts = obj.read_current_attempts + 1;
                data = obj.read_sample();
                obj.read_current_attempts = 0;
            else
               % Retorna somente os dados de acelerometro, giro e magnetometro
               data = data(3:11);
            end

            obj.time_reading = obj.time_reading + toc(t1);
       end

        % Fecha o arquivo ou conexao com porta serial
        function delete(obj)
            fclose(obj.f_pt);
            fprintf('Tempo m�dio de leitura: %fs\n', obj.time_reading / obj.count_reader_times);
        end
    end
end

