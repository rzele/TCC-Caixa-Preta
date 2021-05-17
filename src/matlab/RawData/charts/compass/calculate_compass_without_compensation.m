function ret = calculate_compass_without_compensation(H)
    newYaw = atan2(-H(2), H(1)) * 180/pi;
    if newYaw > 360
        newYaw = newYaw - 360;
    elseif newYaw < 0
        newYaw = newYaw + 360;
    end
    ret = newYaw;
end