% Sobre filtros


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%% Escolher filtro
mm=4;       %Tamanho do m�dia m�vel
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

na=10000;   %Qtd de amostras
fs=100;     %Freq de amostragem
tp=1:na;    
tp=tp/fs;           %Eixo dos tempos
freq=1:na;
freq=freq/(na/(2*fs));  %Eixo dos tempos


x=rand(1,na);   %Gerar ru�do
x=x-mean(x);    %Retirar o valor m�dio (DC)

figure(1);
plot(tp,x,'b'); %Sinal no tempo
title('x(t):Sinal Original');
xlabel('Segundos');
ylabel('Amplitude');

X=fft(x);               %Calcular a FFT

figure(2);
plot(freq,abs(X),'b');  %Sinal em freq
title('X(freq):Espectro do Sinal Original');
xlabel('Freq (Hz)');
ylabel('M�dulo');

%Aplicar filtro M�dia M�vel
aux=zeros(1,mm);
y=x;
ix=1;   %indexador para a mem�ria do filtro
for ct=1:na
    aux(1,ix)=x(1,ct);
    y(1,ct)=sum(aux)/mm;
    ix=ix+1;
    if ix>mm
        ix=1;
    end
end

figure(3);
plot(tp,y,'b'); %Sinal no tempo
title('y(t):Sinal Filtrado');
xlabel('Segundos');
ylabel('Amplitude');

Y=fft(y);               %Calcular a FFT

figure(4)
plot(freq,abs(Y),'b');  %Sinal em freq
title('Y(freq):Espectro do Sinal Filtrado');
xlabel('Freq (Hz)');
ylabel('M�dulo');
