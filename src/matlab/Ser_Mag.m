% Ser_Mag
% Ler dados do Magnetômetro pela porta serial
% Espera CxPreta enviar #[, ASCII 0x23,0x5B
% fclose(instrfind) --> fechar porta
% Os dados escritos no arquivo são
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% asax
% asay
% asaz
% offsetx
% offsety
% offsetz
% escalax
% escalay
% escalaz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Para obter o valor, use a expressão
%
% Hx = (Hx_sensor - offsetx)*escalax 
%
% UNIDADES
% uT: Hx * 4912.0f / 32760.0
% G:  Hx * (4912.0f / 32760.0)/100
%




clear all;
%fclose(instrfind);  %Fechar possível porta serial
close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Usuário selecionar sua janela
janela=5;       %Tamanho da janela em segundos
passo=0.5;      %Passo da janela em segundos
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fprintf(1,'Teste 17\n');
fprintf(1,'Matlab receber dados do Magnetômetro da Caixa Preta.\n');
fprintf(1,'Em caso de erro, a porta serial fica travada.\n');
fprintf(1,'Neste caso use "fclose(instrfind)" para fechar porta serial.\n');

%Parâmetros
fa=100;         %Frequência de amostragem em Hz
ta=1/fa;        %Intervalo entre amostras (periodo)


%Cálculos para a janela, em amostras
tam=janela*fa;   %Tamanho da janela em nr de amostras
pa=fa*passo;    %Passo em nr de amostras

sid=serial('COM4','Baudrate',115200);
fopen(sid);
if (sid==-1)
    fprintf(1,'Nao abriu COM4.\n');
    return;
end

x1=0;
x2=0;

hx=zeros(1,tam);
hy=hx;
hz=hx;
eixo=0:ta:janela-ta;
plot(eixo,hx,'b');

pause(2);

fprintf(sid,'t17\r\n');

% Esperar sinal da Caixa Preta
fprintf(1,'\nPor favor, selecione Teste 17 na Caixa Preta.\n');
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
pause(1);

asax=fscanf(sid,'%d',10);
asay=fscanf(sid,'%d',10);
asaz=fscanf(sid,'%d',10);

figure(1);
%figure('Name','Recepção dados do MPU');
uhx=0;  %Último hx
uhy=0;  %Último hy
uhz=0;  %Último hz
ix=1;


while uhx~=22222 | uhy~=22222
    uhx=fscanf(sid,'%d',10);
    uhy=fscanf(sid,'%d',10);
    uhz =fscanf(sid,'%d',10);
    hx(ix)=uhx;
    hy(ix)=uhy;
    hz(ix)=uhz;
    if ix<tam
        if mod(ix,pa) == 0
            
            
            %Medidas Mag
            subplot(3,1,1);
            plot(eixo,hx,'b');
            %axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('h');
            grid;
            msg=sprintf('hx [%.1f s]',ix/100);
            title(msg);
            subplot(3,1,2);
            plot(eixo,hy,'b');
            %axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('h');
            grid;
            title('hy');
            subplot(3,1,3);
            plot(eixo,hz,'b');
            %axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('h');
            grid;
            title('hz');
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
            
            %Medidas Mag
            subplot(3,1,1);
            plot(eixo,hx(1,ini:fim),'b');
            %axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('h');
            grid;
            msg=sprintf('hx [%.1f s]',ix/100);
            title(msg);
            subplot(3,1,2);
            plot(eixo,hy(1,ini:fim),'b');
            %axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('h');
            grid;
            title('hy');
            subplot(3,1,3);
            plot(eixo,hz(1,ini:fim),'b');
            %axis([eixo(1) eixo(tam) -esc_ac esc_ac]);
            ylabel('h');
            grid;
            title('hz');
            drawnow;

        end
    end
    ix=ix+1;
end
ix=ix-1;
fprintf(1,'\nTeminou recepção de dados.\n');
fclose(sid);
fprintf(1,'Recebidas %d leituras por eixo.\n',ix);
fprintf(1,'Duração %.2f segundos.\n',ix/fa);
%close all;

% Remover a marca final "22222" de todos os eixos
% Repete a penúltima leitura
hx(1,ix)=hx(1,ix-1);
hy(1,ix)=hy(1,ix-1);
hz(1,ix)=hz(1,ix-1);

figure(2);
plot(hx,hy,'or',hx,hz,'og',hy,hz,'ob');

% Hard Iron - Remover offset
hx_off=(max(hx)+min(hx))/2;
hy_off=(max(hy)+min(hy))/2;
hz_off=(max(hz)+min(hz))/2;
hx_hi=hx-hx_off;
hy_hi=hy-hy_off;
hz_hi=hz-hz_off;
figure(3);
plot(hx_hi,hy_hi,'or',hx_hi,hz_hi,'og',hy_hi,hz_hi,'ob');

% Soft Iron
hx_avg=(max(hx)-min(hx))/2;
hy_avg=(max(hy)-min(hy))/2;
hz_avg=(max(hz)-min(hz))/2;
avg=(hx_avg+hy_avg+hz_avg)/3;
hx_sc=avg/hx_avg;
hy_sc=avg/hy_avg;
hz_sc=avg/hz_avg;

%Correção completa
hx_f=(hx-hx_off)*hx_sc;
hy_f=(hy-hy_off)*hy_sc;
hz_f=(hz-hz_off)*hz_sc;

%converte para uT
hx_f = hx_f * 4912.0 / 32760;
hy_f = hy_f * 4912.0 / 32760;
hz_f = hz_f * 4912.0 / 32760;

%escreve em arquivo offsets e escalas para cada eixo
fName= 'calibragem.txt';

fid=fopen(fName,'wt');
%Verificar se abriu o arquivo
if (fid==-1)
    fprintf(1,'Nao abriu arquivo [%s]. Parar!\n',fName);
    return;
end

fprintf(fid, '%f\n', asax);
fprintf(fid, '%f\n', asay);
fprintf(fid, '%f\n', asaz);

fprintf(fid, '%f\n', hx_off);
fprintf(fid, '%f\n', hy_off);
fprintf(fid, '%f\n', hz_off);

fprintf(fid, '%f\n', hx_sc);
fprintf(fid, '%f\n', hy_sc);
fprintf(fid, '%f\n', hz_sc);


fclose(fid);


figure(4);
plot(hx_f,hy_f,'or',hx_f,hz_f,'og',hy_f,hz_f,'ob');

return;


% ((((float)mag_asa[i]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f
