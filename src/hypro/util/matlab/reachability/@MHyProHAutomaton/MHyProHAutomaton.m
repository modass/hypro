classdef MHyProHAutomaton < handle
    
    properties (SetAccess = public, GetAccess = public)
        Handle
    end
    
    methods (Access = private)
        
        % Destructor
        function delete(obj)
            MHyPro(4, 1, obj.Handle);
        end
    end
    
    methods (Access = public)
        
        % Constructor
        function obj = MHyProHAutomaton(varargin)
            if nargin == 0
                obj.Handle = MHyPro(4, 2);
            elseif nargin == 1
                if isa(varargin{1}, 'uint64')
                    obj.Handle = varargin{1};
                elseif isa(varargin{1}, 'MHyProHAutomaton')
                    obj.Handle = MHyPro(4, 3, varargin{1}.Handle);
                else
                    obj.Handle = MHyPro(4, varargin{1});
                end
            elseif nargin == 2
                % 4
            elseif nargin == 3
                obj.Handle = MHyPro(4, varargin{1}, varargin{2}, varargin{3});
            end
        end
                
        function out = dimension(obj)
            out = MHyPro(4, 5, obj.Handle);
        end
        
        function out = decompose(obj, vec)
            out = MHyPro(4, 6, obj.Handle, vec);
        end
        
        function out = getLocations(obj)
            locations = MHyPro(4, 7, obj.Handle);
            len = length(locations);
            out = cell(1,len);
            for i = 1:length(len)
                out{i} = MHyProLocation(locations{i});
            end
        end
        
        function out = getLocations_by_hash(obj, hash)
            if isreal(hash)
                locations = MHyPro(4, 8, obj.Handle, hash);
                len = length(locations);
                out = cell(1,len);
                for i = 1:length(len)
                    out{i} = MHyProLocation(locations{i});
                end
            else
                error('MHyProHAutomaton - getLocations_by_hash: Wrong type of argument.');
        
            end
        end
        
        function out = getLocations_by_name(obj, name)
            if isstring(name)
                locations = MHyPro(4, 9, obj.Handle, name);
                len = length(locations);
                out = cell(1,len);
                for i = 1:length(len)
                    out{i} = MHyProLocation(locations{i});
                end
            else
                error('MHyProHAutomaton - getLocations_by_name: Wrong type of argument.');
            end
        end
        
        function out = getTransitions(obj)
            transitions = MHyPro(4, 10, obj.Handle);
            len = length(transitions);
            out = cell(1,len);
            for i = 1:length(len)
                out{i} = MHyProTransition(transitions{i});
            end
        end
        
        function out = getInitialStates(obj)
           mapping = MHyPro(4, 11, obj.Handle);
            len = length(mapping);
            out = cell(1,len);
            for i = 1:length(len)
                out{i}.location = MHyProLocation(mapping(i).location);
                out{i}.condition = MHyProCondition(mapping(i).condition);
            end
        end
        
        function out = getLocalBadStates(obj)
            mapping = MHyPro(4, 12, obj.Handle);
            len = length(mapping);
            out = cell(1,len);
            for i = 1:length(len)
                out{i}.location = MHyProLocation(mapping{i}.location);
                out{i}.condition = MHyProCondition(mapping{i}.condition);
            end
        end
        
        function out = getGlobalBadStates(obj)
            mapping = MHyPro(4, 13, obj.Handle);
            len = length(mapping);
            out = cell(1,len);
            for i = 1:length(len)
                out{i}.location = MHyProLocation(mapping{i}.location);
                out{i}.condition = MHyProCondition(mapping{i}.condition);
            end
        end
        
        function out = getVariables(obj)
            out = MHyPro(4, 14, obj.Handle);
        end
        
        function out = getLables(obj)
            labels = MHyPro(4, 15, obj.Handle);
            len = length(labels);
            out = cell(1,len);
            for i = 1:length(len)
                out{i} = MHyProLocation(labels{i});
            end
        end
        
        function setLocations(obj, locs)
            if iscell(locs) && isa(locs{0}, 'MHyProLocation')
                len = lengt(locs);
                loc_handles = cell(1,len);
                for i = 1:len
                    loc_handles{i} = locs{i}.Handle;
                end
                MHyPro(4, 16, obj.Handle, loc_handles);
            end
        end
        
        function setInitialStates(obj, mapping)
            len = length(mapping);
            mapping_handles = cell(1,len);
            for i = 1:len
                mapping_handles{i}.location = mapping{i}.location.Handle;
                mapping_handles{i}.condition = mapping{i}.condition.Handle;
            end
            MHyPro(4, 17, obj.Handle, mapping_handles);
        end
        
        function setLocalBadStates(obj, mapping)
            MHyPro(4, 18, obj.Handle);
            %TODO
        end
        
        function setGlobalBadStates(obj, mapping)
            MHyPro(4, 19, obj.Handle);
            %TODO
        end
        
        function setVariables(obj, vars)
            MHyPro(4, 20, obj.Handle);
        end
        
        function addLocation(obj, loc)
            if isa(loc, 'MHyProLocation')
                MHyPro(4, 21, obj.Handle, loc.Handle);
            end
        end
        
        function addInitialState(obj, loc, cond)
            if isa(loc, 'MHyProLocation') && isa(cond, 'MHyProCondition')
                MHyPro(4, 22, obj.Handle, loc.Handle, cond.Handle);
            else
                error('MHyProHAutomaton - addInitialState: Wrong type of an argument.');
            end
        end
        
        function addLocalBadState(obj, loc, cond)
            if isa(loc, 'MHyProLocation') && isa(cond, 'Condition')
                MHyPro(4, 23, obj.Handle, loc.Handle, cond.Handle);
            else
                error('MHyProHAutomaton - addLocalBadState: Wrong type of an argument.');
            end
        end
        
        function addGlobalBadState(obj, cond)
            if isa(cond, 'Condition')
                MHyPro(4, 24, obj.Handle, cond.Handle);
            else
                error('MHyProHAutomaton - addGlobalBadState: Wrong type of an argument.');
            end
        end
        
        function reduce(obj)
            MHyPro(4, 25, obj.Handle);
        end
        
        function out = isComposedOf(obj, rhs)
            if isa(rhs, 'MHyProAutomaton')
                 out = MHyPro(4, 26, obj.Handle, rhs.Handle);
            else
                error('MHyProHAutomaton - isComposedOf: Wrong type of an argument.');
            end
        end
        
        function out = getDotRepresentation(obj)
            out = MHyPro(4, 27, obj.Handle);
        end
        
        function out = getStatistics(obj)
            out = MHyPro(4, 28, obj.Handle);
        end
        
        function out = eq(lhs, rhs)
            if isa(lhs, 4) && isa(rhs, 4)
                out = MHyPro(4, 29, lhs.Handle, rhs.Handle);
            else
                error('MHyProHAutomaton - ==: Wrong type of an argument.');
            end
        end
        
        function out = nq(lhs, rhs)
           if isa(lhs, 4) && isa(rhs, 4)
                out = MHyPro(4, 30, lhs.Handle, rhs.Handle);
            else
                error('MHyProHAutomaton - !=: Wrong type of an argument.');
            end
        end 
        
        function out = plus(lhs, rhs)
            if isa(lhs, 4) && isa(rhs, 4)
                out = MHyPro(4, 31, lhs.Handle, rhs.Handle);
            else
                error('MHyProHAutomaton - +: Wrong type of an argument.');
            end
        end
     
    end
    
end