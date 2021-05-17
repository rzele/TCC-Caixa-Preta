function ret = subAngle( ang1, ang2 )
    % Subtrai ângulos (ang1 - ang2)
    % O objetivo é, dados ângulos quaisquer, obter a soma correta no intervalo -180, +180 
    % Essas transformações geram erro em ponto flutuante de +-0.005
    a = cosd(ang1 - ang2);
    b = sind(ang1 - ang2);

    ret = atan2(b,a) * 180/pi;
end

