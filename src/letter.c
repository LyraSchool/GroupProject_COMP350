main()
{
	int i,j,k=1,l;
	char* msg="Letter is A\r\n";

	enableInterrupts();
	for(i=0; i<26; i++)
	{
		syscall(0,msg);
		for(j=0; j<300; j++)
		{
			for(l=0; l<10; l++)
				k=k*2;
		}
		msg[10]++;
	}
	syscall(5);
	while(1);
}
