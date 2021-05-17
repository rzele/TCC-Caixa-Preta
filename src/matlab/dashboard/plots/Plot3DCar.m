classdef Plot3DCar < CustomPlot
    %Plot3DCar Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        g
        patch
        last_q = false;

        p_title
    end
    
    methods
        function obj = Plot3DCar(p_title)
            obj.p_title = p_title;
        end

        % Define as propriedades do objeto no layout (titulo, legenda...)
        function createPlot(obj, n_rows, n_columns, grid)
            obj.grid = grid;
            obj.n_rows = n_rows;
            obj.n_columns = n_columns;
            
            obj.setCar();
            title(obj.p_title)
        end

        % Define o carro
        function setCar(obj)
            verts = [ 3 -1 -1; ...
                      3 -1  0; ...
                      2 -1  0; ...
                      1 -1  1; ...
                      -1 -1  1; ...
                      -1 -1  0; ...
                      -3 -1  0; ...
                      -3 -1 -1; ...
                      3  1 -1; ...
                      3  1  0; ...
                      2  1  0; ...
                      1  1  1; ...
                      -1  1  1; ...
                      -1  1  0; ...
                      -3  1  0; ...
                      -3  1 -1];
     
            faces = [1   2   3   4   5   6   7   8   ; ... % Lateral esquerda
                    9   10  11  12  13  14  15  16  ; ... % Lateral direita
                    1   2   10  9   nan nan nan nan ; ... % Para choque dianteiro
                    2   3   11  10  nan nan nan nan ; ... % Cap�
                    3   4   12  11  nan nan nan nan ; ... % Vidro dianteiro
                    4   5   13  12  nan nan nan nan ; ... % Teto
                    5   6   14  13  nan nan nan nan ; ... % Vidro traseiro
                    6   7   15  14  nan nan nan nan ; ... % Carroceria
                    7   8   16  15  nan nan nan nan ; ... % Para choque traseiro
                    8   1   9   16  nan nan nan nan ; ... % Baixo
                    ];
                
            colors = [[1    0    0    ] ; ... % Lateral esquerda
                    [1    0    0    ] ; ... % Lateral direita
                    [0.1  0.1  0.1  ] ; ... % Para choque dianteiro
                    [1    0    0    ] ; ... % Cap�
                    [0.85 0.85 0.85 ] ; ... % Vidro dianteiro
                    [1    0    0    ] ; ... % Teto
                    [0.85 0.85 0.85 ] ; ... % Vidro traseiro
                    [0.2  0.2  0.2  ] ; ... % Carroceria
                    [0.1  0.1  0.1  ] ; ... % Para choque traseiro
                    [0.3  0.3  0.3  ] ; ... % Baixo
                    ];

            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);
            obj.g = hgtransform;
            obj.patch = patch('Vertices',verts,'Faces',faces, 'FaceVertexCData', colors, 'FaceColor','flat','Parent', obj.g);
            view(3)
            box on
            axis([-4 4 -4 4 -4 4])
            daspect([1 1 1])
        end

        function rotateWithEuler(obj, xAng, yAng, zAng)
            x = xAng * pi / 180;
            y = yAng * pi / 180;
            z = zAng * pi / 180;
            
            Rx = makehgtform('xrotate',x);
            Ry = makehgtform('yrotate',y);
            Rz = makehgtform('zrotate',z);

            obj.g.Matrix = Rz * Ry * Rx;
        end

        function rotateWithQuaternion(obj, q)
            if (obj.last_q)
                q_diff = quatmultiply(q, quatinv(obj.last_q));
            else
                q_diff = q;
            end

            c = quatern2axisAngle(quatnormalize(q_diff));

            rotate(obj.patch, c.axis, c.angle);
            
            obj.last_q = q;
        end

        function on_update(obj)
        end

        function on_delete(obj)
        end
    end
    
end

