classdef MovingMedian < handle
    properties 
        data = []
        k
    end    

    methods
        function obj = MovingMedian(k, n_elemets)
            obj.k = k;
            obj.data = zeros(k, n_elemets);
        end

        function ret = calculate(obj, newData)
            if obj.k == 1
                ret = newData;
                return
            end
                
            obj.data = [obj.data(2:obj.k,:) ; newData];
            ordained = sort(obj.data);
            ret = ordained(ceil(obj.k/2), :);
        end
    end
end