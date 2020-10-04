classdef EmptyPlot < CustomPlot
    % Esta classe é usada para mocar os items não definidos no layout, descartando a necessidade de verificação ao usar os items
    
    properties
    end
    
    methods
        function setProperties(obj, p_title, p_xlabel, p_ylabel, s_legend)
        end

        function setSource(obj, sources_name, sources_color)
        end

        function setCar(obj)
        end

        function rotateWithEuler(obj)
        end
        
        function rotateWithQuaternion(obj)
        end

        function setCompass(obj)
        end

        function rotateCompass(obj)
        end
        
        function update(obj, source)
        end
        
    end
    
end

