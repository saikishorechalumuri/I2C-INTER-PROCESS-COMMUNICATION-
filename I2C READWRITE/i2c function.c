
void I2C1_write(int address)
{	
while((I2C1_SR2 & 0x2)); //busy
I2C1_CR1 |=(0x1<<8);//start condition
	
while((I2C1_SR1 & (0x1))==0); //SB
clear =	I2C1_SR1;// clear SB	
	
I2C1_DR = (0x50<<1); // EEPROM slave address
//I2C1_CR1 &= ~(0x1<<8);	//CLEAR START BIT
while(((I2C1_SR1 & (0x2))==0)  && ((I2C1_SR1 & (0x1<<10))==0));	 //ADDR ------ AF
if((I2C1_SR1 & 0x2))
	{
	        clear = I2C1_SR1;
		clear = I2C1_SR2;	
		LCD_WRITE_COMREG(0x01);//clear screen
		LCD_WRITE_COMREG(0x80); //SET CURSOR POSITION
		LCD_WRITE_STRING("device FOUND");
	}
	else
	{
		I2C1_CR1 |= (0x1<<9); // stop bit generated
		I2C1_SR1 &= ~(0x1<<10);// clear AF
	}
	while(!(I2C1_SR1 & (0x1<<7))){;}//wait until txe bit set
	I2C1_DR = address;  //memory address
	size = sizeof(Buff);
	for(i=address;i<(8+(1*address));i++)	
		{
			 while(!(I2C1_SR1 & (0x1<<7))){;}//wait until txe bit is set
			I2C1_DR =  Buff[i];
		
		}
I2C1_CR1 |=(0x200);//stop	
I2C1_CR1 &=~(0x1<<10); // clear AF
}

void I2C1_Read()
{
	 I2C1_CR1 &= ~(0x1); //PE disable
 	 I2C1_CR1 |=0x1; //PE
	 I2C1_CR1 |= (0x1<<10); //ACK
		while((I2C1_SR2 & 0x2)); //busy
		I2C1_CR1 |=(0x1<<8);//start condition

		while((I2C1_SR1 & (0x1))==0); //SB
		clear =	I2C1_SR1;// clear SB	
		
		I2C1_DR = (0xA0); // RTC slave address

		//I2C1_CR1 &= ~(0x1<<8);	//CLEAR START BIT
		while(((I2C1_SR1 & (0x2))==0)  && ((I2C1_SR1 & (0x1<<10))==0));	 //ADDR ------ AF
		if((I2C1_SR1 & 0x2))
		{
			clear = I2C1_SR1;
			clear = I2C1_SR2;	
		        
			while(!(I2C1_SR1 & (0x1<<7))){;}//wait until txe bit set
		        I2C1_DR = 0x00; //address
			I2C1_CR1 |= (0x1<<8); //Restart
		
			while((I2C1_SR1 & (0x1))==0); //SB
			clear =	I2C1_SR1;// clear SB
					
			I2C1_DR =0xA1;// slave address(0x50<<1) + read mode(1)
					
			while(((I2C1_SR1 & (0x2))==0)  && ((I2C1_SR1 & (0x1<<10))==0));	 //ADDR ------ AF
			if((I2C1_SR1 & 0x2))
			{
					clear = I2C1_SR1;
					clear = I2C1_SR2;	
			}
			size--;
		        for(i=0;i<size;i++)
			{
					while(!(I2C1_SR1 & (0x1<<6))){;}//wait until rxne bit is set
					data[i]= I2C1_DR ; 	
		 	}
		}	
			
		I2C1_CR1 &=~(0x1<<10); //ACK
		I2C1_CR1 |=(0x1<<9);//stop
		//delaysec(1);
		//	data[7]='\0';
		delayms(10);
		LCD_WRITE_COMREG(0x80); //SET CURSOR POSITION 
		LCD_WRITE_STRING(data);				
}
