function ret = calculate_aceleration(A, a_bias, esc_ac)
    %% Converter aceleração em "g"
    ax = esc_ac * (A(1) / 32767) - a_bias(1);
    ay = esc_ac * (A(2) / 32767) - a_bias(2);
    az = esc_ac * (A(3) / 32767) - a_bias(3);

    ret = [ax, ay, az];
end