% Importa tudo que esta na mesma raiz que a main.m, incluindo pastas e subpastas
addpath(genpath('.'))

close all;                          % close all figures
clear;                              % clear all variables
clc;                                % clear the command terminal

%% 1 - Obtem configurações estáticas
cnf = Configs();

%% 1.5 - Gera arquivos falsos, se configurado
if strcmp(cnf.read_from, 'mockup')
    df = DataFactory(cnf.file_full_path, cnf.fake_sample_freq, cnf.fake_esc_ac, cnf.fake_esc_giro, cnf.debug_on);
    cnf.file_full_path = df.generate();

    if cnf.only_generate
        return
    end
end

%% 2 - Abre porta serial / arquivo
if strcmp(cnf.read_from, 'serial') || strcmp(cnf.read_from, 'bluethoot')
    reader = ReaderSerial(cnf.serial_COM, cnf.serial_baudrate);
elseif strcmp(cnf.read_from, 'arquivo') || strcmp(cnf.read_from, 'mockup')
    reader = ReaderFile(cnf.file_full_path);
else
    error('Invalid parameter "read_from". Change it at config file');
end

%% 3 - Atualiza as configurações com metadados da caixa preta
cnf.setDynamicConfigs(reader.metadatas);

%% 4 - Instância todo o conjunto de gráficos
charts = Charts(cnf);

%% 5 - Obtem a configuração do layout
cnf.setLayout(charts);

%% 6 - Define uma janela p/ plot
plot_1 = Render(cnf.freq_render, cnf.layout);

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

    % Chama cada gráfico na tela para realizar seus calculos
    % atualizando os dados a partir dos dados de entrada
    for i = 1:length(plot_1.plots)
        plot_1.plots(i).calculate(data, count);
    end
    
    time_calc_data = time_calc_data + toc(t1);

    %% Tenta redesenhar os graficos na tela com os novos dados, se deu o tempo da frequencia
    % Este redesenho frequente da tela ainda não é algo bem modelado, podendo causar lentidões e travamento
    % desabilite o 'plot_in_real_time' nas configurações para melhor performance
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