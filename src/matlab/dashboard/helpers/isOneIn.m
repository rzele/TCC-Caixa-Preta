function isIn = isOneIn( first_list, second_list )
% Check if the second_list (cell array) has some item inside the firs list (cell array)
%   
    isIn = false;

    for i = 1:length(second_list)
        if any(strcmp(first_list, second_list(i)))
            isIn = true;
            break
        end
    end
end