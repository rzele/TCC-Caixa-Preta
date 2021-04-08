function ret = integrate(accumulated, y1, y0, delta_x)
    ret = accumulated + (delta_x * (y0 + y1) / 2);
end