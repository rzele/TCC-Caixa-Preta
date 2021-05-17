function isIn = isAllIn( first_list, second_list )
% Check if the second_list (cell array) is all inside the firs list (cell array)
%   
    isIn = true;

    for i = 1:length(second_list)
        if ~any(strcmp(first_list, second_list(i)))
            isIn = false;
            break
        end
    end
end

