function ret = calculate_aceleration_without_gravity(tilt, A)
    Rot_M = ang2rotZYX(tilt(1), tilt(2), tilt(3));

    unrotated_a = Rot_M * [A(1); A(2); A(3)];

    % Remove gravity vector (1g)
    unrotated_a(3) = unrotated_a(3) - 1;

    ret = unrotated_a';
end