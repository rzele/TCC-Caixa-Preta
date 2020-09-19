classdef ReaderSerial < ReaderCore
    %READERSERIAL Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        serial_com
        serial_baudrate
    end
    
    methods
        function obj = ReaderSerial(serial_com, serial_baudrate)

            % Se a porta serial ja estiver aberta tenta fechar
            try
                fclose(instrfind);
            catch ME
            end
            
            obj.serial_com = serial_com;
            obj.serial_baudrate = serial_baudrate;
            obj.start_delimiter = '#[m';
            obj.end_delimiter = 'm]#';
            obj.data_delimiter = ' ';

            obj.f_pt = serial(serial_com, 'Baudrate', serial_baudrate);
            fopen(obj.f_pt);

            if (obj.f_pt==-1)
                fprintf(1,'Nao abriu COM.\n');
                return
            end

            % Manda sinal p/ o arduino iniciar a leitura no teste12
            pause(2)
            fprintf(obj.f_pt, 't12\r\n');

            obj.wait_start_signal();
            obj.set_metadatas();
        end

        function delete(obj)
            % Manda sinal p/ o arduino parar a leitura no teste12
            fprintf(obj.f_pt, 'x\r\n');
        end
    end
    
end

