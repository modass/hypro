classdef MHyProLocation < handle
    
    properties (SetAccess = public, GetAccess = public)
        Handle
    end
    
    methods (Access = private)
        
        % Destructor
        function delete(obj)
            MHyPro(6, 1, obj.Handle);
        end
    end
    
    methods (Access = public)
        
        %Constructor
         function obj = MHyProLocation(varargin)
            if nargin == 0
                % Call the constructor for empty flow
                obj.Handle = MHyPro(6, 2);
            elseif nargin == 1
                if isa(varargin{1}, 'uint64')
                    obj.Handle = varargin{1};
                elseif isa(varargin{1}, 'MHyProLocation')
                    obj.Handle = MHyPro(6, 3, varargin{1}.Handle);
                elseif ismatrix(varargin{1})
                    % Call the matrix constructor
                    obj.Handle = MHyPro(6, 4, varargin{1});
                else
                    error('MHyProLocation - Constructor: Wrong type of at least one argument.');
                end
            elseif nargin == 3
                if ismatrix(varargin{1}) && iscell(varargin{2}) && isa(varargin{3}, 'MHyProCondition')
                    objects = cell(1, size(varargin{2}, 2));
                    for i = 1:length(varargin{2})
                        if isa(varargin{2}{i}, 'MHyProTransition')
                            objects{i} = varargin{2}{i}.Handle;
                        else
                            error('MHyProBox - unite: Wrong type of input argument.');
                        end
                    end
                    % Call the matrix-vector constructor for affine flow
                    obj.Handle = MHyPro(6, 5 ,varargin{1}, objects, varargin{3}.Handle);
                else
                    error('MHyProLocation - Constructor: Wrong type of at least one argument.');
                end
            else
                error('MHyProLocation - Constructor: Wrong type of at least one argument.');
            end
         end
        
        function out = getNumberFlow(obj)
            out = MHyPro(6, 6, obj.Handle);
        end
        
        function out = getLinearFlow(obj)
            ptr = MHyPro(6, 7, obj.Handle);
            out = MHyProFlow(9, ptr);
        end
        
        function out = getLinearFlows(obj)
            ptrscell = MHyPro(6, 8, obj.Handle);
            out = cell(1, size(ptrscell, 2));
            for i = 1:size(ptrscell,2)
                ptr = ptrscell{i};
                out{i} = MHyProFlow(9, ptr);
            end
        end
        
        function out = getInvariant(obj)
            ptr = MHyPro(6, 9, obj.Handle);
            out = MHyProCondition(ptr);
        end
        
        function out = getTransitions(obj)
            ptrscell = MHyPro(6, 10, obj.Handle);
            out = cell(1, size(ptrscell,2));
            for i = 1:size(ptrscell,2)
                ptr = ptrscell{i};
                out{i} = MHyProTransition(ptr);
            end
        end
        
        function out = getExternalInput(obj)
            if obj.hasExternalInput()
                out = MHyPro(6, 11, obj.Handle);
            else
                error('MHyProLocation - getExternalInput: Object has no external input.');
            end
        end
        
        function out = hasExternalInput(obj)
            out = MHyPro(6, 12, obj.Handle);
        end
        
        function out = hash(obj)
            out = MHyPro(6, 13, obj.Handle);
        end
        
        function out = getName(obj)
            out = MHyPro(6, 14, obj.Handle);
        end
        
        function out = dimension(obj)
            out = MHyPro(6, 15, obj.Handle);
        end
        
        function out = dimension_at(obj, pos)
            if mod(pos,1) == 0
                out = MHyPro(6, 16, obj.Handle, pos);
            else
                error('MHyProLocation - dimension_at: Wrong type of at leat one argument. Note: strings in MATLAB have to be written in ''.');
            end
        end
        
        function setName(obj, name)
            if ischar(name)
                MHyPro(6, 17, obj.Handle, name);
            else
                error('MHyProLocation - setName: Wrong type of at leat one argument. Note: strings in MATLAB have to be written in ''.');
            end
        end
        
        function setFlow(obj, mat)
            if ismatrix(mat)
                MHyPro(6, 18, obj.Handle, mat);
            else
                error('MHyProLocation - setFlow: Wrong type of at leat one argument.');
            end
        end
        
        function setLinearFlow(obj, linFlow, pos)
            if isa(linFlow, 'MHyProFlow') && linFlow.Type == 9 && mod(pos,1) == 0
                MHyPro(6, 19, obj.Handle, linFlow.Handle, pos - 1);
            else
                error('MHyProLocation - setLinearFlow: Wrong type of at leat one argument.');
            end
        end
        
        function setLinearFlows(obj, linFlows)
            if iscelloftype(linFlows, 9) 
                MHyPro(6, 20, obj.Handle, linFlows);
            else
                error('MHyProLocation - setLinearFlows: Wrong type of at leat one argument.');
            end
        end
           
        function setInvariant(obj, cond)
            if isa(cond, 'MHyProCondition') 
                MHyPro(6, 21, obj.Handle, cond.Handle);
            else
                error('MHyProLocation - setInvariant: Wrong type of at leat one argument.');
            end
        end
        
        function setExtInput(obj, ints)
            if areIntervals(ints)
                MHyPro(6, 22, obj.Handle, ints);
            else
                error('MHyProLocation - setExtInput: Wrong type of at leat one argument.');
            end
        end
        
        function addTransition(obj, tran)
            if isa(tran, 'MHyProTransition')
                MHyPro(6, 23, obj.Handle, tran.Handle);
            else
                error('MHyProLocation - addTransition: Wrong type of at leat one argument.');
            end
        end
        
        function out = isComposedOf(obj)
            %TODO 24
            out = 0;
        end
        
        function out = getDotRepresentation(obj, vars)
            if isstring(vars)
                out = MHyPro(6, 25, obj.Handle, vars);
            else
                error('MHyProLocation - getDotRepresentation: Wrong type of at leat one argument.');
            end
        end
        
        function out = decompose(obj)
            %TODO 26
        end
        
        function out = lt(obj, rhs)
            if isa(rhs, 'MHyProLocation')
                out = MHyPro(6, 27, obj.Handle, rhs.Handle);
            else
                error('MHyProLocation - less: Wrong type of at leat one argument.');
            end
        end
        
        function out = eq(obj, rhs)
            if isa(rhs, 'MHyProLocation')
                out = MHyPro(6, 28, obj.Handle, rhs.Handle);
            else
                error('MHyProLocation - equals: Wrong type of at leat one argument.');
            end
        end
        
        function out = ne(obj, rhs)
            if isa(rhs, 'MHyProLocation')
                out = MHyPro(6, 29, obj.Handle, rhs.Handle);
            else
                error('MHyProLocation - unequals: Wrong type of at leat one argument.');
            end
        end
        
    end
end