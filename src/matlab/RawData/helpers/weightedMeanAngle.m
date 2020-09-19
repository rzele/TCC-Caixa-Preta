function ret = weightedMeanAngle( ang1, ang2, mu )
% Check if the second_list (cell array) is all inside the firs list (cell array)
%  
    a1 = sind(ang1);
    b1 = cosd(ang1);
    a2 = sind(ang2);
    b2 = cosd(ang2);
    a = (1-mu)*a1 + mu*a2;
    b = (1-mu)*b1 + mu*b2;

    ret = atan2(a,b) * 180/pi;
end

