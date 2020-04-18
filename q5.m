clear all;

esc_ac=2;

nome='q5data.txt';
fid=fopen(nome,'r');
%Verificar se abriu o arquivo
if (fid==-1)
    fprintf(1,'Nao abriu arquivo [%s]. Parar!\n',nome);
    return;
end

%dados = fscanf(fid,'%d',1500); %fiz 500 leituras, mas são 3 eixos
dados = fscanf(fid,'%d');
fclose(fid);

%fprintf('%d\n', dados);

total = size(dados,1);
total_leituras = total/3;

ax = zeros(1,total_leituras);
ay = ax;
az = ax;

for i=1:total_leituras
    ax(1,i) = dados( ((i-1)*3)+1,1 );
    ay(1,i) = dados( ((i-1)*3)+2,1 );
    az(1,i) = dados( ((i-1)*3)+3,1 );
end

% Converter acelerações em "g"
ax=esc_ac*(ax/32767);
ay=esc_ac*(ay/32767);
az=esc_ac*(az/32767);

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

return;



