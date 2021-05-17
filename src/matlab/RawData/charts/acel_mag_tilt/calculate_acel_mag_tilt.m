function ret = calculate_acel_mag_tilt(a, h)
    % Normaliza aceler�metor e megnet�metro
    A = a / norm(a);
    H = h / norm(h);

    %% Calcula pich e roll usado acelera��o
    % O uso da fun��o real() aqui � para obter a parte real do n�mero
    % por algum motivo o matlab estava retornando um n�mero complexo para o caso onde A(1) = 1.000...
    newPitch = real(asin(-A(1))) * 180/pi;
    newRoll = atan2(A(2), A(3)) * 180/pi;

    %% Calcula yaw usando magnet�metro (compass com compensa��o)
    % Ref do calculo: https://www.mikrocontroller.net/attachment/292888/AN4248.pdf
    x = H(1)*cosd(newPitch) + H(2)*sind(newRoll)*sind(newPitch) + H(3)*cosd(newRoll)*sind(newPitch);
    y = H(2)*cosd(newRoll) - H(3)*sind(newRoll);

    newYaw = atan2(-y, x) * 180/pi;

    ret = [newRoll, newPitch, newYaw];
end