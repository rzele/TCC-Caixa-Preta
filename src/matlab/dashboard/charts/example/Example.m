classdef Example < CommonsLine
    % Esta é uma classe de exemplo, crie uma cópia da pasta
    % e faça suas modificações para criar seu próprio gráfico
    % 
    % Esta classe é do tipo 'cálculo' e do tipo 'plot'
    % Sendo assim, cabe a ela realizar cálculos e ao mesmo tempo
    % renderizar os dados na tela em 'gráficos de linha'.
    % A este tipo hibrido de classe demos o nome de 'chart'
    %
    % Por conta desta classe herdar 'CommonsLine', ela já é do tipo 'PlotLine'
    % e alguns métodos e heranças já estão implementados
    % veja o README.md e a CommonsLine.m para mais detalhes

    properties
        % propriedades deste chart, que ainda não foram definidos no CommonsLine
    end

    methods
        function obj = Example(w_size)
            % Esta função é o construtor da classe, e por ser 
            % do tipo 'PlotLine', sempre recebe 'w_size' como parâmetro

            % Instância a classe 'CommonsLine', que irá instânciar o 'PlotLine'
            % definindo assim parâmetros do gráfico exibido na tela
            % Note que aqui foi passado como parâmetro o w_size, ela é utilizada para ditar o tamanho
            % do eixo no gráfico do tipo 'PlotLine'
            obj = obj@CommonsLine(...
                'exemplo', ...                          % p_title
                'Amostra', ...                          % p_xlabel
                'ex', ...                               % p_ylabel
                {'x', 'y', 'z'}, ...                    % s_legend
                {'r', 'g', 'b'}, ...                    % sources_color
                w_size);

            % 'w_size' é o tamanho da janela, é importante salva-lo 
            % ele está definido na classe pai
            obj.w_size = w_size;

            % 'data' são os dados após calculos realizados por esta classe.
            % Note que ele tem o tamanho de w_size. Como o programa pode ser utilizado
            % em tempo real, e não será exibido 100% dos dados na tela (por 
            % questões de visualição), não tem porque armazenar todo o histórico.
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            % Esta função é chamada pela main, no momento de calcular uma nova amostra
            % ou por classes que tenham essa como depedência.
            % Ela irá receber os dados acel, gyro, mag do mpu
            % e posição e tilt(ZYX) do baseline (se definido o modo de leitura como 'mockup').
            % Também recebe 'n_sample', para saber a qual amostra (primeira, segunda, terceira?)
            % esses dados se referem

            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)

                % O método 'has_calculated_this_sample' é implementado pela classe pai
                % é importante essa chamada pois evita o re-calculo.
                % 
                % Uma vez que os gráficos podem ser injetados um nos outros e,
                % não se sabe a ordem que eles iram executar, nem se seram chamados pela main,
                % um gráfico que possui dependência sempre irá chamar suas
                % dependências para calcular a amostra 'i', desta forma é possível
                % ter certeza que ao chamar o método de get irá retornar o dado calculado desta amostra 'i'.
                %
                % E.x.: Para calcular o tilt relativo (integral da velocidade angular)
                % É necessário ter convertido os dados do MPU, quem faz isso é a classe Gyroscope.
                % Sendo assim, tilt relativo depende de Gyroscope.
                % Logo, ele vai pedir p/ Gyroscope calcular a amostra i, em seguida irá utilizar esse dado
                % para calcular o tilt relativo
                %
                % NOTA: imagine que todas as funções de calculo são máquinas de estado
                % e que não podem voltar ao estado anterior, sendo assim, não tem como pedir para ela calcular
                % 'i-1'. Para isso elas são obrigadas, pelo 'Template.m', a salvar não só
                % seu estado atual ('last'), como também seu estado anterior ('penult') e se solicitado
                % retornar esse dado
 
                return
            end

            %% Calcula o valor p/ a próxima amostra
            % aqui você pode realizar a lógica que precisar para calcular seu dado
            
            % Esta última linha irá salvar o dado calculado para esta amostra.
            % Cada classe pode salvar como quiser seu dado, desde que respeite a interface
            % definida em 'Template.m', porém como aqui estamos herdando a 'CommonsLine'
            % iremos seguir o padrão p/ utilizar as implementações úteis que ela tem.
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end