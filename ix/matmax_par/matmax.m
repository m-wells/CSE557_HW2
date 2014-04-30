A = rand(8000);
B = rand(8000);
C = rand(8000);

A = distributed(A);
B = distributed(B);
C = distributed(C);


tic;
C = A*B+C;
toc
  fprintf('time %d',toc)
  quit;
