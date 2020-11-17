void PTM(double* O, double* A, double* B1, double* B2, double* B3, double* B4, double* F, double* C, double eps)
{
	int i, j, m0, m1, m2, m3, m4, ii;
	int it = 0;
	//	int t=t+ht;
	double max;
	int kol_del = 5;
	int num_sek, st, fin, ob;
	double Arr, rr, RrRr, Ar, Rr;
	double omega = 0.0;
	double tau = 0.0;
	int N1, N2;
	MPI_Status status;

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	N1 = world_rank * (Ny - 2) / world_size;
	N2 = (world_rank + 1) * (Ny - 2) / world_size + 2;
	N2 = N2 - N1;
	int N_par = N2 * Nx;

	double* buf = new double[1];
	double* rez = new double[world_size];
	double* vec_in = new double[Nx];
	double* vec_out = new double[Nx];

	//Здесь хранится невязка
	double* r = new double[N_par];

	for (i = 0;i < N_par;i++) r[i] = 0;

	//передача вправо
	if (world_rank < (world_size - 1))
	{
		for (ii = 1;ii < Nx - 1;ii++)
		{
			m0 = ii * N2 + N2 - 2;
			vec_out[ii] = A[m0];
		}
		MPI_Send(vec_out, Nx, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD);
	}

	if (world_rank > 0)
	{
		MPI_Recv(vec_in, Nx, MPI_DOUBLE, world_rank - 1, 0, MPI_COMM_WORLD, &status);
		for (ii = 1;ii < Nx - 1;ii++)
		{
			m0 = ii * N2;
			A[m0] = vec_in[ii];
		}
	}

	//передача вправо
	if (world_rank < (world_size - 1))
	{
		for (ii = 1;ii < Nx - 1;ii++)
		{
			m0 = ii * N2 + N2 - 2;
			vec_out[ii] = B1[m0];
		}
		MPI_Send(vec_out, Nx, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD);
	}

	if (world_rank > 0)
	{
		MPI_Recv(vec_in, Nx, MPI_DOUBLE, world_rank - 1, 0, MPI_COMM_WORLD, &status);
		for (ii = 1;ii < Nx - 1;ii++)
		{
			m0 = ii * N2;
			B1[m0] = vec_in[ii];
		}
	}

	//передача вправо
	if (world_rank < (world_size - 1))
	{
		for (ii = 1;ii < Nx - 1;ii++)
		{
			m0 = ii * N2 + N2 - 2;
			vec_out[ii] = B3[m0];
		}
		MPI_Send(vec_out, Nx, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD);
	}

	if (world_rank > 0)
	{
		MPI_Recv(vec_in, Nx, MPI_DOUBLE, world_rank - 1, 0, MPI_COMM_WORLD, &status);
		for (ii = 1;ii < Nx - 1;ii++)
		{
			m0 = ii * N2;
			B3[m0] = vec_in[ii];
		}
	}

	do
	{
		//Расчет неязки и её нормы
		max = 0;
		rr = Arr = RrRr = 0.0;

		for (i = 1;i < Nx - 1;i++)
			for (j = 1;j < N2 - 1;j++)
			{
				m0 = i * N2 + j;
				m1 = m0 + N2;
				m2 = m0 - N2;
				m3 = m0 + 1;
				m4 = m0 - 1;


				r[m0] = F[m0] - A[m0] * C[m0] + B1[m0] * C[m1] + B2[m0] * C[m2] + B3[m0] * C[m3] + B4[m0] * C[m4];

				if (max < fabs(r[m0]))
					max = fabs(r[m0]);
			}

		buf[0] = max;
		MPI_Allgather(buf, 1, MPI_DOUBLE, rez, 1, MPI_DOUBLE, MPI_COMM_WORLD);


		for (i = 0;i < world_size;i++)
			if (max < rez[i])max = rez[i];

		//1
		for (i = 1;i < Nx - 1;i++)
		{
			num_sek = int(((i - 1) * kol_del) / (Nx - 2));
			st = int((num_sek * (Nx - 2)) / kol_del) + 1;
			fin = int(((num_sek + 1) * (Nx - 2)) / kol_del);
			ob = fin - st + 1;

			if ((st == i) && (world_rank > 0))
			{
				MPI_Recv(vec_in, ob, MPI_DOUBLE, world_rank - 1, 0, MPI_COMM_WORLD, &status);
				for (ii = 0;ii < ob;ii++)
				{
					m0 = (st + ii) * N2;
					r[m0] = vec_in[ii];
				}
			}


			for (j = 1;j < N2 - 1;j++)
			{
				m0 = i * N2 + j;
				m2 = m0 - N2;
				m4 = m0 - 1;

				if (A[m0] > 0)
					r[m0] = (r[m0] + (B2[m0] * r[m2] + B4[m0] * r[m4]) * omega) / (A[m0] * (1 + omega / 2));

			}

			if ((i == fin) && (world_rank < (world_size - 1)))
			{
				for (ii = 0;ii < ob;ii++)
				{
					m0 = (st + ii) * N2 + N2 - 2;
					vec_out[ii] = r[m0];
				}
				MPI_Send(vec_out, ob, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD);
			}

		}
		//2
		for (i = 1;i < Nx - 1;i++)
			for (j = 0;j < N2 - 1;j++)
			{
				m0 = i * N2 + j;
				r[m0] = r[m0] * A[m0];
			}
		//3
		for (i = Nx - 2;i > 0;i--)
		{
			num_sek = int(((i - 1) * kol_del) / (Nx - 2));
			st = int((num_sek * (Nx - 2)) / kol_del) + 1;
			fin = int(((num_sek + 1) * (Nx - 2)) / kol_del);
			ob = fin - st + 1;

			if ((i == fin) && (world_rank < (world_size - 1)))
			{
				MPI_Recv(vec_in, ob, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD, &status);
				for (ii = 0;ii < ob;ii++)
				{
					m0 = (st + ii) * N2 + N2 - 1;
					r[m0] = vec_in[ii];
				}
			}


			for (j = N2 - 2;j >= 0;j--)
			{
				m0 = i * N2 + j;
				m1 = m0 + N2;
				m3 = m0 + 1;

				if (A[m0] > 0)
					r[m0] = (r[m0] + (B1[m0] * r[m1] + B3[m0] * r[m3]) * omega) / (A[m0] * (1 + omega / 2));

			}

			if ((st == i) && (world_rank > 0))
			{
				for (ii = 0;ii < ob;ii++)
				{
					m0 = (st + ii) * N2 + 1;
					vec_out[ii] = r[m0];
				}
				MPI_Send(vec_out, ob, MPI_DOUBLE, world_rank - 1, 0, MPI_COMM_WORLD);
			}

		}


		for (i = 1;i < Nx - 1;i++)
			for (j = 1;j < N2 - 1;j++)
			{
				m0 = i * N2 + j;
				m1 = m0 + N2;
				m2 = m0 - N2;
				m3 = m0 + 1;
				m4 = m0 - 1;


				Rr = 0.5 * A[m0] * r[m0] - B1[m0] * r[m1] - B3[m0] * r[m3];
				Ar = A[m0] * r[m0] - B1[m0] * r[m1] - B2[m0] * r[m2] - B3[m0] * r[m3] - B4[m0] * r[m4];
				rr = rr + A[m0] * r[m0] * r[m0];
				Arr = Arr + Ar * r[m0];
				if (A[m0] > 0) RrRr = RrRr + Rr * Rr / A[m0];
			}

		buf[0] = rr;
		MPI_Allgather(buf, 1, MPI_DOUBLE, rez, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		rr = 0;
		for (i = 0;i < world_size;i++)
			rr = rr + rez[i];

		buf[0] = Arr;
		MPI_Allgather(buf, 1, MPI_DOUBLE, rez, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		Arr = 0;
		for (i = 0;i < world_size;i++)
			Arr = Arr + rez[i];

		buf[0] = RrRr;
		MPI_Allgather(buf, 1, MPI_DOUBLE, rez, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		RrRr = 0;
		for (i = 0;i < world_size;i++)
			RrRr = RrRr + rez[i];

		if (RrRr > 0) omega = sqrt(rr / RrRr);
		if (Arr > 0) tau = omega + 2 * (rr / Arr);

		for (i = 0;i < Nx;i++)
			for (j = 0;j < N2;j++)
			{
				m0 = i * N2 + j;

				C[m0] = C[m0] + tau * r[m0];
			}


		it++;

	} while (max > eps);

	if (world_rank == 0)	printf("t=%f it=%d \n", t, it);
	delete[]r;
}