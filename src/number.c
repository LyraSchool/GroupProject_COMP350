main()
{
	int i,j,k=1,l;
	char* msg="Number is 0\r\n";

	enableInterrupts();
	for(i=0; i<100; i++)
	{
		msg[10]++;
		if(msg[10]==0x3a)
			msg[10]='0';
		syscall(0,msg);
		for(j=0; j<10000; j++)
		{
			for(l=0; l<10; l++)
				k=k*2;
		}
	}
	syscall(5);
	while(1);
}
