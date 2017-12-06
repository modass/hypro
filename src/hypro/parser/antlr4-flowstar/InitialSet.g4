/*
 * InitialSet.g4
 *
 * @author Phillip Tse
 * @date 20.6.2017
 *
 * A grammar for AnTLR to generate a parser for the initial sets of a hybrid automaton.
 */

grammar InitialSet;

options { tokenVocab = FormulaLexer; }

import FormulaParser;

//////// Parser Rules

init 	 		: 'init' '{' initstate+ '}' ;	

initstate 		: VARIABLE '{' constrset '}' ;