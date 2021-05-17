function ret = rot2angZYX(Rot)
    %% Extrai Yaw, Pitch e Roll absolutos(em relação a Posição inicial do corpo) p/ a nova amostra usando a atual matriz de Rotação (Z,Y,X)
    % Ref do calculo: https://www.youtube.com/watch?v=wg9bI8-Qx2Q
    % 
    % Repare que no video essa extração é p/ a ordem XYZ, mas
    % segundo https://www.geometrictools.com/Documentation/EulerAngles.pdf, seção 2.6
    % Essa extração é para ZYX, e é o que faz sentido
    % 
    % Não me lembro por que utilizar o tan2 p/ extração do do pitch

    roll = atan2(Rot(3,2), Rot(3,3)) * 180/pi;
    yaw = atan2(Rot(2,1), Rot(1,1)) * 180/pi;
    if cosd(yaw) == 0
        pitch = atan2(-Rot(3,1), Rot(2,1)/sind(yaw)) * 180/pi;
    else
        pitch = atan2(-Rot(3,1), Rot(1,1)/cosd(yaw)) * 180/pi;
    end

    ret = [roll pitch yaw];
end

