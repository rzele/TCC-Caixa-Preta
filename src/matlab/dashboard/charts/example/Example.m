classdef Example < CommonsLine
    % Esta � uma classe de exemplo, crie uma c�pia da pasta
    % e fa�a suas modifica��es para criar seu pr�prio gr�fico
    % 
    % Esta classe � do tipo 'c�lculo' e do tipo 'plot'
    % Sendo assim, cabe a ela realizar c�lculos e ao mesmo tempo
    % renderizar os dados na tela em 'gr�ficos de linha'.
    % A este tipo hibrido de classe demos o nome de 'chart'
    %
    % Por conta desta classe herdar 'CommonsLine', ela j� � do tipo 'PlotLine'
    % e alguns m�todos e heran�as j� est�o implementados
    % veja o README.md e a CommonsLine.m para mais detalhes

    properties
        % propriedades deste chart, que ainda n�o foram definidos no CommonsLine
    end

    methods
        function obj = Example(w_size)
            % Esta fun��o � o construtor da classe, e por ser 
            % do tipo 'PlotLine', sempre recebe 'w_size' como par�metro

            % Inst�ncia a classe 'CommonsLine', que ir� inst�nciar o 'PlotLine'
            % definindo assim par�metros do gr�fico exibido na tela
            % Note que aqui foi passado como par�metro o w_size, ela � utilizada para ditar o tamanho
            % do eixo no gr�fico do tipo 'PlotLine'
            obj = obj@CommonsLine(...
                'exemplo', ...                          % p_title
                'Amostra', ...                          % p_xlabel
                'ex', ...                               % p_ylabel
                {'x', 'y', 'z'}, ...                    % s_legend
                {'r', 'g', 'b'}, ...                    % sources_color
                w_size);

            % 'w_size' � o tamanho da janela, � importante salva-lo 
            % ele est� definido na classe pai
            obj.w_size = w_size;

            % 'data' s�o os dados ap�s calculos realizados por esta classe.
            % Note que ele tem o tamanho de w_size. Como o programa pode ser utilizado
            % em tempo real, e n�o ser� exibido 100% dos dados na tela (por 
            % quest�es de visuali��o), n�o tem porque armazenar todo o hist�rico.
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            % Esta fun��o � chamada pela main, no momento de calcular uma nova amostra
            % ou por classes que tenham essa como deped�ncia.
            % Ela ir� receber os dados acel, gyro, mag do mpu
            % e posi��o e tilt(ZYX) do baseline (se definido o modo de leitura como 'mockup').
            % Tamb�m recebe 'n_sample', para saber a qual amostra (primeira, segunda, terceira?)
            % esses dados se referem

            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)

                % O m�todo 'has_calculated_this_sample' � implementado pela classe pai
                % � importante essa chamada pois evita o re-calculo.
                % 
                % Uma vez que os gr�ficos podem ser injetados um nos outros e,
                % n�o se sabe a ordem que eles iram executar, nem se seram chamados pela main,
                % um gr�fico que possui depend�ncia sempre ir� chamar suas
                % depend�ncias para calcular a amostra 'i', desta forma � poss�vel
                % ter certeza que ao chamar o m�todo de get ir� retornar o dado calculado desta amostra 'i'.
                %
                % E.x.: Para calcular o tilt relativo (integral da velocidade angular)
                % � necess�rio ter convertido os dados do MPU, quem faz isso � a classe Gyroscope.
                % Sendo assim, tilt relativo depende de Gyroscope.
                % Logo, ele vai pedir p/ Gyroscope calcular a amostra i, em seguida ir� utilizar esse dado
                % para calcular o tilt relativo
                %
                % NOTA: imagine que todas as fun��es de calculo s�o m�quinas de estado
                % e que n�o podem voltar ao estado anterior, sendo assim, n�o tem como pedir para ela calcular
                % 'i-1'. Para isso elas s�o obrigadas, pelo 'Template.m', a salvar n�o s�
                % seu estado atual ('last'), como tamb�m seu estado anterior ('penult') e se solicitado
                % retornar esse dado
 
                return
            end

            %% Calcula o valor p/ a pr�xima amostra
            % aqui voc� pode realizar a l�gica que precisar para calcular seu dado
            
            % Esta �ltima linha ir� salvar o dado calculado para esta amostra.
            % Cada classe pode salvar como quiser seu dado, desde que respeite a interface
            % definida em 'Template.m', por�m como aqui estamos herdando a 'CommonsLine'
            % iremos seguir o padr�o p/ utilizar as implementa��es �teis que ela tem.
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end