function ret = str2int16( values )
%STR2INT16 Summary of this function goes here
%   Convert string (uint16 or int16) to int16

    if isa(values, 'char')
        num_val = str2num(values);
        if(num_val > 0)
            ret = typecast(uint16(num_val), 'int16');
        else
            ret = num_val;
        end
    elseif isa(values, 'cell')
        ret = [];
        for i=1:length(values)
            num_val = str2num(cell2mat(values(i)));
            if num_val > 0
                ret(i) = typecast(uint16(num_val), 'int16');
            else
                ret(i) = num_val;
            end
        end
    else
        Error('invalid argument type, use string or array-cell')
    end

end

