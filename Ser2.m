% Ser2
% Ler da porta serial
% Espera CxPreta enviar #[, ASCII 0x23,0x5B
% fclose(instrfind) --> fechar porta


clear all;
%fclose(instrfind);  %Fechar possível porta serial
close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Usuário selecionar sua janela
janela=5;       %Tamanho da janela em segundos
passo=0.5;      %Passo da janela em segundos
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fprintf(1,'Teste 12\n');
fprintf(1,'Matlab receber dados da Caixa Preta.\n');
fprintf(1,'Em caso de erro, a porta serial fica travada.\n');
fprintf(1,'Neste caso use "fclose(instrfind)" para fechar porta serial.\n');

%Parâmetros
fa=100;         %Frequência de amostragem em Hz
ta=1/fa;        %Intervalo entre amostras (periodo)
esc_ac=2;
esc_gi=250;


%Cálculos para a janela, em amostras
tam=janela*fa;   %Tamanho da janela em nr de amostras
pa=fa*passo;    %Passo em nr de amostras

sid=serial('COM5','Baudrate',500000);
fopen(sid);
if (sid==-1)
    fprintf(1,'Nao abriu COM5.\n');
    break;
end

x1=0;
x2=0;

ax=zeros(1,tam);
ay=ax;
az=ax;
tp=ax;
gx=ax;
gy=ax;
gz=ax;
eixo=0:ta:janela-ta;
plot(eixo,ax,'b');

% Esperar sinal da Caixa Preta
fprintf(1,'\nPor favor, selecione Teste 12 na Caixa Preta.\n');
while x1~='#' | x2~='['
    x1=x2;
    x2=fread(sid,1);
    fprintf(1,'%c',x2);
    %fprintf(1,'[x1:%H=%c]',x1,x1);
    %fprintf(1,' [x2:%H=%c]\n',x2,x2);
end

%Chegou sinal, iniciar recepção de dados
fprintf(1,'==> Padrão esperado.');
fprintf(1,'\nIniciando recepção de dados...\n');
figure(1);
%figure('Name','Recepção dados do MPU');
ix=1;
while ax~=22222 | ay~=22222
    ax(ix)=fscanf(sid,'%d',10);
    ay(ix)=fscanf(sid,'%d',10);
    az(ix)=fscanf(sid,'%d',10);
    tp(ix)=fscanf(sid,'%d',10);
    gx(ix)=fscanf(sid,'%d',10);
    gy(ix)=fscanf(sid,'%d',10);
    gz(ix)=fscanf(sid,'%d',10);
    if ix<tam
        if mod(ix,pa) == 0
            
            %Acelerações;
            subplot(3,2,1);
            plot(eixo,(esc_ac*ax)/32767,'b');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('g');
            grid;
            msg=sprintf('ax [%.1f s]',ix/100);
            title(msg);
            subplot(3,2,3);
            plot(eixo,(esc_ac*ay)/32767,'b');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('g');
            grid;
            title('ay');
            subplot(3,2,5);
            plot(eixo,(esc_ac*az)/32767,'b');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('g');
            grid;
            title('az');
            drawnow;

            %Giros;
            subplot(3,2,2);
            plot(eixo,(esc_gi*gx)/32767,'b');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('graus/s');
            grid;
            title('gx');
            subplot(3,2,4);
            plot(eixo,(esc_gi*gy)/32767,'b');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('graus/s');
            grid;
            title('gy');
            subplot(3,2,6);
            plot(eixo,(esc_gi*gz)/32767,'b');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('graus/s');
            grid;
            title('gz');
            drawnow;

        end
    end
    if ix>(tam+pa)
        if mod(ix,pa) == 0
            ti=(ix-tam)/fa;
            tf=ix/fa;
            eixo=ti:ta:tf-ta;
            ini=ix-tam;
            fim=ix-1;
            
            %Acelerações
            subplot(3,2,1);
            plot(eixo,(esc_ac*ax(1,ini:fim)/32767),'b');
            ylabel('g');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            grid;
            msg=sprintf('ax [%.1f s]',ix/100);
            title(msg);
            subplot(3,2,3);
            plot(eixo,(esc_ac*ay(1,ini:fim)/32767),'b');
            ylabel('g');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            title('ay');
            grid;
            subplot(3,2,5);
            plot(eixo,(esc_ac*az(1,ini:fim)/32767),'b');
            ylabel('g');
            axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            title('az');
            grid;
            drawnow;
            
            %Giros
            subplot(3,2,2);
            plot(eixo,(esc_gi*gx(1,ini:fim)/32767),'b');
            axis([eixo(1) eixo(tam) -esc_gi esc_gi]);
            grid;
            ylabel('graus/s');
            title('gx');
            subplot(3,2,4);
            plot(eixo,(esc_gi*gy(1,ini:fim)/32767),'b');
            axis([eixo(1) eixo(tam) -esc_gi esc_gi]);
            ylabel('graus/s');
            title('gy');
            grid;
            subplot(3,2,6);
            plot(eixo,(esc_gi*gz(1,ini:fim)/32767),'b');
            axis([eixo(1) eixo(tam) -esc_gi esc_gi]);
            ylabel('graus/s');
            title('gz');
            grid;
            drawnow;

        end
    end
    %fprintf(1,'Qtd=%d leituras: ax=%d ay=%d az=%d tp=%d gx=%d gy=%d gz=%d\n',ix,ax(ix),ay(ix),az(ix),tp(ix),gx(ix),gy(ix),gz(ix));
    ix=ix+1;
end
ix=ix-1;
fprintf(1,'\nTeminou recepção de dados.\n');
fclose(sid);
fprintf(1,'Recebidas %d leituras por eixo.\n',ix);
fprintf(1,'Duração %.2f segundos.\n',ix/fa);
close all;

% Remover a marca final "22222" de todos os eixos
% Repete a penúltima leitura
ax(1,ix)=ax(1,ix-1);
ay(1,ix)=ay(1,ix-1);
az(1,ix)=az(1,ix-1);
gx(1,ix)=gx(1,ix-1);
gy(1,ix)=gy(1,ix-1);
gz(1,ix)=gz(1,ix-1);
tp(1,ix)=tp(1,ix-1);

% Converter acelerações em "g"
ax=esc_ac*(ax/32767);
ay=esc_ac*(ay/32767);
az=esc_ac*(az/32767);

% Converter giros em "graus/seg"
gx=esc_gi*(gx/32767);
gy=esc_gi*(gy/32767);
gz=esc_gi*(gz/32767);

% Converter temperatura para Celsius
tp=(tp/340)+36.53;

% Desenhar gráficos
%ax
figure(1);
plot(ax);
grid;
title('Aceleração: eixo X em g');
xlabel('Amostra');
ylabel('g');

%ay
figure(2);
plot(ay);
grid;
title('Aceleração: eixo Y em g');
xlabel('Amostra');
ylabel('g');

%az
figure(3);
plot(az);
grid;
title('Aceleração: eixo Z em g');
xlabel('Amostra');
ylabel('g');

%gx
figure(4);
plot(gx);
grid;
title('Giro: eixo X em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

%gy
figure(5);
plot(gy);
grid;
title('Giro: eixo Y em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

%gz
figure(6);
plot(gz);
grid;
title('Giro: eixo Z em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

%Temperatura
figure(7);
plot(tp);
grid;
title('Temperatura em graus Celsius');
xlabel('Amostra');
ylabel('graus Celsius');

figure(1);

%Aqui acaba o script
break

% Como ler arquivo texto com o Matlab
nome='T1_50K.txt';
fid=fopen(nome,'r');
%Verificar se abriu o arquivo
if (fid==-1)
    fprintf(1,'Nao abriu arquivo [%s]. Parar!\n',nome);
    break;
end
w=fscanf(fid,'%d ');
fclose(fid);


