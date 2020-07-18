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
    end
    
end

