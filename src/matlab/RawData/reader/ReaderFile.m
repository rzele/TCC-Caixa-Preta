classdef ReaderFile < ReaderCore
    %READERFILE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        file_full_path
    end
    
    methods
        function obj = ReaderFile(file_full_path)
            obj.file_full_path = file_full_path;
            obj.start_delimiter = '#[m';
            obj.end_delimiter = 'm]#';
            obj.data_delimiter = ' ';

            [obj.f_pt, err] = fopen(file_full_path, 'r', 'n', 'UTF-8');

            if (obj.f_pt==-1)
                fprintf(1,'Nao abriu arquivo de nome: %s.\n', file_full_path);
                fprintf(1,'%s.\n', err);
                return
            end
            fprintf('Buscando sinal de start\n');

            obj.wait_start_signal();
            obj.get_and_set_metadatas();
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

        end
    end
    
end

