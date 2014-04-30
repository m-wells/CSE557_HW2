A = rand(8000);
B = rand(8000);
C = rand(8000);

tic;
C = A*B+C;
toc
  fprintf('time %d',toc)
  quit;
