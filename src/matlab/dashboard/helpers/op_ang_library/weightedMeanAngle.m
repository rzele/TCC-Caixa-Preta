function ret = weightedMeanAngle( ang1, ang2, mu )
    % Essas transformações geram erro em ponto flutuante de +-0.005
    a1 = cosd(ang1);
    b1 = sind(ang1);
    a2 = cosd(ang2);
    b2 = sind(ang2);
    a = (1-mu)*a1 + mu*a2;
    b = (1-mu)*b1 + mu*b2;

    ret = atan2(b,a) * 180/pi;
end

