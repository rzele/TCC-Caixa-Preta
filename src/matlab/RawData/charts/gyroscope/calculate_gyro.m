function ret = calculate_gyro(G, g_bias, esc_giro)
    %% Converter giros em "graus/seg"
    gx = esc_giro * (G(1) / 32767) - g_bias(1);
    gy = esc_giro * (G(2) / 32767) - g_bias(2);
    gz = esc_giro * (G(3) / 32767) - g_bias(3);

    ret = [gx, gy, gz];
end