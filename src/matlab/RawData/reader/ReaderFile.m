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

            obj.wait_start_signal();
            obj.get_and_set_metadatas();
        end
    end
    
end

