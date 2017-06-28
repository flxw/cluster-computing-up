int i, hits, x, y;
hits = 0;
result = 0;
#pragma omp parallel private(hits)
{
    hits = 0;
    #pragma omp for
    for (i = 0; i < num_samples; i++) {
        x = random();
        y = random();
        if (x * x + y * y <= 1)
        {
            hits++;
        }
    }
    #pragma omp critical;
    result += hits;
}