classdef ReaderCore < handle
    %READERCORE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        f_pt
        stop_wait = false
        read_attempts = 5
        read_current_attempts = 0
        start_delimiter 
        end_delimiter 
        data_delimiter 
        metadatas = struct(                         ...
            'colision_date', 'ddmmyy',              ...
            'colision_time', 'hhmmss',              ...
            'SRAM_adr', 0,                          ...
            'n_samples', 0                         ...
        )
    end
    
    methods
        function obj = ReaderCore(start_delimiter, end_delimiter, data_delimiter)
        end

        function wait_start_signal(obj)
            while ~obj.stop_wait
                temp = fgetl(obj.f_pt);
                if strcmp(temp, obj.start_delimiter) == 1
                    obj.stop_wait = true;
                    break;
                end
            end
            fprintf('Iniciando leitura.\n');
        end

        function get_and_set_metadatas(obj)
            obj.metadatas.('colision_date') = obj.read_not_empty_line();
            obj.metadatas.('colision_time') = obj.read_not_empty_line();
            obj.metadatas.('SRAM_adr') = str2num(obj.read_not_empty_line());
            obj.metadatas.('n_samples') = str2num(obj.read_not_empty_line());
        end

        function force_stop_wait(obj)
            obj.stop_wait = true;
        end

        function line_str = read_not_empty_line(obj)
            is_empty_line = true;

            while is_empty_line
                % Le ate achar quebra de linha, e remove espaços em branco
                line_str = fgetl(obj.f_pt);
                line_str = strrep(line_str, ' ', '');

                if length(line_str) > 1
                    is_empty_line = false;
                end
            end
        end

        function data = read_sample(obj)
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
            if length(data) < 6
                if (obj.read_current_attempts == obj.read_attempts)
                    data = [];
                    return;
                end
                obj.read_current_attempts = obj.read_current_attempts + 1;
                data = obj.read_sample();
                obj.read_current_attempts = 0;
            end
        end

        % Fecha o arquivo ou conexao com porta serial
        function delete(obj)
            fclose(obj.f_pt);
        end
    end
end

