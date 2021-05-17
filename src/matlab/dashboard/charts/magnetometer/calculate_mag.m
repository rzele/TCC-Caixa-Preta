function ret = calculate_mag(H, h_offsets, h_scales)
    %% Remove hard and soft iron dos dados do magnetometro
    hx = (H(1) - h_offsets(1)) * h_scales(1);
    hy = (H(2) - h_offsets(2)) * h_scales(2);
    hz = (H(3) - h_offsets(3)) * h_scales(3);

    %% Converter leitura do magnetometro em micro Testla p/ mili Gaus
    % Trocando a ordem, porque os eixos do mag são X p/ Y do giro, Y p/ X
    % do giro e -Z p/ Z do giro
    temp_x = hy;
    temp_y = hx;
    temp_z = -hz;
    hx = (4912 * temp_x/32767) * 10;
    hy = (4912 * temp_y/32767) * 10;
    hz = (4912 * temp_z/32767) * 10; 

    ret = [hx, hy, hz];
end