% Ler da porta serial
% Ainda precisa melhorar
% BUGS:
% - file_simulated_freq ainda não funciona muito bem

addpath('quaternion_library');      % include quaternion library
addpath('reader');                  % include reader library
addpath('helpers');                 % include some useful functions
addpath('charts');                  % 
addpath('plots');                   % include plot library

close all;                          % close all figures
clear;                              % clear all variables
clc;                                % clear the command terminal

%% 1 - Obtem configurações estáticas
cnf = Configs();

%% 2 - Abre porta serial / arquivo
if cnf.read_from_serial
    reader = ReaderSerial(cnf.serial_COM, cnf.serial_baudrate);
else
    reader = ReaderFile(cnf.file_full_path);
end

%% 3 - Atualiza as configurações com metadados da caixa preta
cnf.setDynamicConfigs(reader.metadatas);

%% 4 - Instância todo o conjunto de gráficos
charts = Charts(cnf);

%% 5 - Obtem a configuração do layout
cnf.setLayout(charts);

%% 6 - Define uma janela p/ plot
plot_1 = Render(cnf.freq_render, cnf.layout);

% Obtem a lista de itens unicos definidos no layout
% para evitar calculo de itens descenessários
setted_objects_id = plot_1.setted_objects_id;

%% Obtem os dados e plota em tempo real
% NOTA: Se o buffer do serial encher (> 512 bytes) o programa pode explodir ou apresentar erros, caso isso ocorra
% abaixe a taxa de renderização do gráfico. Para verificar se erros ocorreram, compare a quantidade de amostras enviadas com a quantidade lida
time_calc_data = 0;
count=0;                                 % conta quantas amostras foram lidas
while true
    
    %% Lê uma amostra de cada da porta serial/arquivo
    data = reader.read_sample();
    
    % Se é o fim do arquivo ou deu algum erro finaliza
    if isempty(data)
        break;
    end
    
    count=count+1;
    
    t1 = tic;
    data = str2int16(data);

    %% Convert data
    charts.aceleration.calculate(data(1:3), [cnf.ax_bias, cnf.ay_bias, cnf.az_bias], cnf.esc_ac);
    charts.gyroscope.calculate(data(4:6), [cnf.gx_bias, cnf.gy_bias, cnf.gz_bias], cnf.esc_giro);
    charts.magnetometer.calculate(data(7:9), [cnf.hx_offset, cnf.hy_offset, cnf.hz_offset], [cnf.hx_scale, cnf.hy_scale, cnf.hz_scale]);


    if isOneIn(setted_objects_id, {charts.gyro_relative_tilt.id, charts.gyro_absolute_tilt.id, charts.acel_mag_tilt.id, charts.comp_tilt.id, charts.car_3d_gdeg.id, charts.car_3d_gtilt.id, charts.car_3d_acelMag.id, charts.car_3d_comp.id, charts.acel_without_g.id, charts.velocity.id, charts.position.id})
        charts.gyro_relative_tilt.calculate(charts.gyroscope.last(), charts.gyroscope.penult(), cnf.freq_sample);
    end
    
    if isOneIn(setted_objects_id, {charts.gyro_absolute_tilt.id, charts.comp_tilt.id, charts.car_3d_gtilt.id, charts.car_3d_acelMag.id, charts.car_3d_comp.id})
        charts.gyro_absolute_tilt.calculate(charts.gyro_relative_tilt.last());
    end
    
    if isOneIn(setted_objects_id, {charts.acel_mag_tilt.id, charts.comp_tilt.id, charts.kalman_tilt.id, charts.acel_without_g.id, charts.velocity.id, charts.position.id, charts.car_3d_acelMag.id, charts.car_3d_kalman.id, charts.car_3d_comp.id, charts.compass_compensated.id})
        charts.acel_mag_tilt.calculate(charts.aceleration.last(), charts.magnetometer.last());
    end

    if isOneIn(setted_objects_id, {charts.compass.id})
        charts.compass.calculate(charts.magnetometer.last());
    end

    if isOneIn(setted_objects_id, {charts.compass_compensated.id})
        acel_mag_last = charts.acel_mag_tilt.last();
        charts.compass_compensated.calculate(acel_mag_last(3));
    end

    if isOneIn(setted_objects_id, {charts.comp_tilt.id, charts.car_3d_comp.id})
        charts.comp_tilt.calculate(charts.gyro_relative_tilt.last(), charts.gyro_relative_tilt.penult(), charts.acel_mag_tilt.last(), cnf.mu); 
    end

    if isOneIn(setted_objects_id, {charts.kalman_tilt.id, charts.car_3d_kalman.id})
        charts.kalman_tilt.calculate(charts.gyroscope.last(), charts.acel_mag_tilt.last());
    end
    
    if isOneIn(setted_objects_id, {charts.Madgwick_tilt.id, charts.car_3d_madgwick.id, charts.quaternion.id})
        charts.Madgwick_tilt.calculate(charts.gyroscope.last(), charts.aceleration.last(), charts.magnetometer.last());
    end

    %% Plota quaterions do filtro de madgwick
    if isOneIn(setted_objects_id, {charts.quaternion.id})
        charts.quaternion.calculate(charts.Madgwick_tilt.last_quaternion());
    end
    
    if isOneIn(setted_objects_id, {charts.acel_without_g.id, charts.velocity.id, charts.position.id})
        charts.acel_without_g.calculate(charts.gyro_relative_tilt.last(), charts.aceleration.last());
    end

    if isOneIn(setted_objects_id, {charts.velocity.id, charts.position.id})
        charts.velocity.calculate(charts.acel_without_g.last(), charts.acel_without_g.penult(), cnf.freq_sample, cnf.const_g);
    end

    if isOneIn(setted_objects_id, {charts.position.id})
        charts.position.calculate(charts.velocity.last(), charts.velocity.penult(), cnf.freq_sample);
    end

    % %% Plota o carro em 3d, podendo ser usado qualquer um dos dados para rotacionar o objeto (Rotação absoluta, relativa, filtro de kalman ...)
    if isOneIn(setted_objects_id, {charts.car_3d_gdeg.id})
        charts.car_3d_gdeg.calculate(charts.gyro_relative_tilt.last());
    end

    if isOneIn(setted_objects_id, {charts.car_3d_gtilt.id})
        charts.car_3d_gtilt.calculate(charts.gyro_absolute_tilt.last());
    end

    if isOneIn(setted_objects_id, {charts.car_3d_acelMag.id})
        charts.car_3d_acelMag.calculate(charts.acel_mag_tilt.last());
    end

    if isOneIn(setted_objects_id, {charts.car_3d_comp.id})
        charts.car_3d_comp.calculate(charts.comp_tilt.last());
    end

    if isOneIn(setted_objects_id, {charts.car_3d_kalman.id})
        charts.car_3d_kalman.calculate(charts.kalman_tilt.last());
    end

    if isOneIn(setted_objects_id, {charts.car_3d_madgwick.id})
        charts.car_3d_madgwick.calculate(charts.Madgwick_tilt.last_quaternion());
    end
    
    time_calc_data = time_calc_data + toc(t1);

    %% Tenta redesenhar o plot, se deu o tempo da frequencia
    if cnf.plot_in_real_time
        plot_1.try_render();
    end
end

%% Renderiza pela ultima vez, independente de ter dado o tempo da frequencia
plot_1.force_render();

reader.delete();
plot_1.delete();

%% Calcula média dos tempos
fprintf('Tempo médio de calculo: %fs\n', time_calc_data / count);

%% Aqui acaba o script
fprintf(1,'Recebidos %d amostras\n\n',count);
return;