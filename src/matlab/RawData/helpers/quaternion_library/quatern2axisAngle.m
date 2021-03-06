function ret = quatern2axisAngle(q)
%QUATERNO2AXISANGLE Converts a quaternion to an axis-angle orientation
%
%   q = quatern2axisAngle(q)
%
%   Converts a quaternion to axis-angle orientation where a 3D rotation
%   is described by an angular rotation around axis defined by a vector.
%
%	Date          Author          Notes
%	06/03/2021    JLG Nogueira    Initial release

    axis = q(:, 2:4) / sin(2*acos(q(:, 1)));
    angle = 2*acos(q(:, 1)) * 180/pi;

    ret = struct('axis', axis, 'angle', angle);
end