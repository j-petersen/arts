%------------------------------------------------------------------------
% NAME:     qtool
%
%           Creates an ARTS control file from a template.
%
%           [!! To be written ... !!]
%
% FORMAT:   [cfile,basename,artscall] = qtool(Q,tmpdir,template [,QE])
%
% OUT:      cfile      Name on control file.
%           basename   ARTS basename.
% IN:       Q          Setting structure.
%           tmpdir     Temporary directory.
%           template   Template file.
% OPTIONAL: QE         Structure with extra settings.
%------------------------------------------------------------------------

% HISTORY: 2001.02.19  Created by Patrick Eriksson.


function [cfile,basename] = qtool(Q,tmpdir,template,QE)

if ~exist('QE')
  QE = [];
end


%=== Create some name strings =================================================
%
basename = [tmpdir,'/out'];
cfile    = [basename,'.arts']; 


%=== Open template reading and cfile for writing
fid_in = fopen(template,'r');
if fid_in < 0
  error(sprintf('Could not open %s for reading.',template))
end
fid_out = fopen(cfile,'w');
if fid_out < 0
  error(sprintf('Could not open %s for writing.',cfile))
end


%=== Read from template, replace keywords etc. and write to cfile
line    = 0;
in_if   = 0;
in_else = 0;
do      = 1;
do_this = 1;

while 1

  s    = fgets(fid_in);
  line = line + 1;

  if isstr(s)

    do_this = 1;

    if (length(s)>=3) & strcmp(s(1:3),'IF ')

      if in_if | in_else
        error(sprintf('Nested IFs found on line %d.',line));
      end

      in_if   = 1;
      in_else = 0;
      s       = deblank(s(4:(length(s)-1)));
      if isempty(s)
        error(sprintf('IF statement without variable found on line %d.',...
                                                                      line));
      end
      if eval(s)
        do = 1;
      else
        do = 0;
      end         
      do_this = 0;

    end %if

    if (length(s)>=4) & strcmp(s(1:4),'ELSE')

      if ~in_if | in_else
        error(sprintf('Not allowed placement of ELSE at line %d.',line));
      end

      in_if   = 0;
      in_else = 1;
      do      = ~do;
      do_this = 0;

    end %else

    if (length(s)>=3) & strcmp(s(1:3),'END')

      if ~in_if & ~in_else 
        error(sprintf('Not allowed placement of END at line %d.',line));
      end

      in_if   = 0;
      in_else = 0;
      do      = 1;
      do_this = 0;

    end %else

    if do_this & do

      %= Check first if any "inline" function shall be called
      if s(1) == '�'

        s = deblank( s );
        eval([ 'qinline_',s(2:length(s)), '(Q,fid_out);' ])

      %= Replace variables (marked by $$) and move text to cfile
      else

	dollars = find( s == '$' );
	if ~isempty(dollars)
	  if isodd(length(dollars))
	    error(sprintf(...
		       'An odd number of $-signs was found on line %d.',line));
	  end
	  while ~isempty(dollars)
	    i1 = dollars(1);
	    i2 = dollars(2);
	    name = s((i1+1):(i2-1));
	    if isstr(eval(name))
	      s = [s(1:(i1-1)),eval(name),s((i2+1):length(s))];
	    else
	      s = [s(1:(i1-1)),num2str(eval(name)),s((i2+1):length(s))];
	    end
	    dollars = find( s == '$' );
	  end
	end
    
	fprintf(fid_out,'%s',s);

      end % else
    end % if do...

  else  % s not a string

    if in_if | in_else
      error('EOF reached inside IF or ELSE statement.');
    end

    break;
  end  

end


fclose(fid_in);
fclose(fid_out);



