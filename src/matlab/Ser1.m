% Ler da porta serial
% Ainda precisa melhorar
clear all;
%nr_lt=10024;   %Total de leituras ax ay az tp gx gy gz   
%qtd=7*nr_lt;  %Total de dados (7 dados em cada leitura)

%Vem do Arduino, função que configura escalas
%mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
esc_ac=2;
esc_giro=250;


sid=serial('COM5','Baudrate',115200);
fopen(sid);
if (sid==-1)
    fprintf(1,'Nao abriu COM5.\n');
    break;
end
fprintf(1,'Pronto para receber dados!\n');


z=0;
cont=0;
tt=0;   %teste
t1=0;
t2=0;
x=[];
%while z<9999
%while cont<qtd
while tt==0
    z=fscanf(sid,'%d',10);
    x=[x z];
    t1=t2;
    t2=z;
    tt=(t1==1)&(t2==2);   %t1=1 e t2=2?
    cont=cont+1;
end
fprintf(1,'Recebidos %d dados\nLeituras = %d.\n',cont,cont/7);
fclose(sid);

%Separar leituras, de 7 em 7
tot=floor(cont/7);
ax=zeros(1,tot);
ay=ax;
az=ax;
tp=ax;
gx=ax;
gy=ax;
gz=ax;
for ii=1:tot
    ax(1,ii) = x( 1, ((ii-1)*7)+1 );
    ay(1,ii) = x( 1, ((ii-1)*7)+2 );
    az(1,ii) = x( 1, ((ii-1)*7)+3 );
    tp(1,ii) = x( 1, ((ii-1)*7)+4 );
    gx(1,ii) = x( 1, ((ii-1)*7)+5 );
    gy(1,ii) = x( 1, ((ii-1)*7)+6 );
    gz(1,ii) = x( 1, ((ii-1)*7)+7 );
end


% Converter acelerações em "g"
ax=esc_ac*(ax/32767);
ay=esc_ac*(ay/32767);
az=esc_ac*(az/32767);

% Converter giros em "graus/seg"
gx=esc_giro*(gx/32767);
gy=esc_giro*(gy/32767);
gz=esc_giro*(gz/32767);

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

%Aqui acaba o script
break

% Como ler arquivo texto
nome='T1_50K.txt';
fid=fopen(nome,'r');
%Verificar se abriu o arquivo
if (fid==-1)
    fprintf(1,'Nao abriu arquivo [%s]. Parar!\n',nome);
    break;
end
w=fscanf(fid,'%d ');
fclose(fid);


