
void Led_On_All(void)
{

}

void Led_Off_All(void)
{
	LED_M1 = LED_OFF;
	LED_M2 = LED_OFF;
	LED_M3 = LED_OFF;
	LED_CHR = LED_OFF;
	LED_CHG = LED_OFF;
}

void Led_Mode_On(void)
{
	LED_M1 = LED_OFF;
	LED_M2 = LED_OFF;
	LED_M3 = LED_OFF;
	switch (WORK_MODE)
		{
		case 1:
			{
			LED_M1 = LED_ON;
			}
			break;
		case 2:
			{
			LED_M2 = LED_ON;
			}
			break;
		case 3:
			{
			LED_M3 = LED_ON;
			}
			break;

		default:
			break;
		}
}

void Led_Hand(void)
{
	if (TIM10MS_LED < 25)
		return;

	if (b_BatLow)		// Bat_Low 1S 4 ´Î
		{
		TIM10MS_LED = 0;
		
		if (LED_CHR)
			{
			LED_CHR = LED_ON;
			LED_CHG = LED_OFF;
			}
		else
			{
			LED_CHR = LED_OFF;
			LED_CHG = LED_OFF;
			}
		return;
		}

	if (TIM10MS_LED < 50)
		return;

	TIM10MS_LED = 0;
	if (b_DcIn && (b_BatFull==0))		// Ch_In 1S 2 ´Î
		{
		if (LED_CHR)
			{
			LED_CHR = LED_ON;
			LED_CHG = LED_OFF;
			}
		else
			{
			LED_CHR = LED_OFF;
			LED_CHG = LED_OFF;
			}
		return;
		}
}


