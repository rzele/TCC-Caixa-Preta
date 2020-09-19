function ret = rot2angZYX(Rot)
% rotation matrix ZYX to euler angles(in deg)
%  
    roll = atan2(Rot(3,2), Rot(3,3)) * 180/pi;
    yaw = atan2(Rot(2,1), Rot(1,1)) * 180/pi;
    if cosd(yaw) == 0
        pitch = atan2(-Rot(3,1), Rot(2,1)/sind(yaw)) * 180/pi;
    else
        pitch = atan2(-Rot(3,1), Rot(1,1)/cosd(yaw)) * 180/pi;
    end

    ret = [yaw; pitch; roll]
end

