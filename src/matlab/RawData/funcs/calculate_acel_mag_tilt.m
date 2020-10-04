function ret = calculate_acel_mag_tilt(A, H)
    %% Calcula pich e roll usado aceleração
    newPitch = atan2(-A(1), sqrt( A(2)^2 + A(3)^2 )) * 180/pi;
    if (A(3)>=0)
        sign = 1;
    else
        sign = -1;
    end
    miu = 0.001;
    newRoll = atan2( A(2),   (sign * sqrt( A(3)^2 + miu * A(1)^2 ))) * 180/pi;

    %% Calcula yaw usando magnetômetro (compass com compensação)
    % Ref do calculo: https://www.mikrocontroller.net/attachment/292888/AN4248.pdf
    x = H(1)*cosd(newPitch) + H(2)*sind(newRoll)*sind(newPitch) + H(3)*cosd(newRoll)*sind(newPitch);
    y = -H(2)*cosd(newRoll) + H(3)*sind(newRoll);

    newYaw = -atan2(-y, x) * 180/pi;

    ret = [newRoll, newPitch, newYaw];
end