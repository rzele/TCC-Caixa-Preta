function ret = subAngle( ang1, ang2 )
    % Subtrai �ngulos (ang1 - ang2)
    % O objetivo �, dados �ngulos quaisquer, obter a soma correta no intervalo -180, +180 
    % Essas transforma��es geram erro em ponto flutuante de +-0.005
    a = cosd(ang1 - ang2);
    b = sind(ang1 - ang2);

    ret = atan2(b,a) * 180/pi;
end

