-- fsm.vhd: Finite State Machine
-- Martin Nizner, xnizne00
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST1, TEST2, TEST3, TEST4, TEST5, TEST6, TEST_FIRST_7,
      TEST_FIRST_8, TEST_FIRST_9, TEST_FIRST_10, TEST_SECOND_7, TEST_SECOND_8,
      TEST_SECOND_9, TEST_SECOND_10, TEST_SECOND_11, TEST_SECOND_12, ERROR, PRINT_OK_MESSAGE,
      PRINT_ERROR_MESSAGE, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(8) = '1') then
         next_state <= TEST2;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
      next_state <= TEST2;
      if (KEY(6) = '1') then
         next_state <= TEST3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST3 =>
      next_state <= TEST3;
      if (KEY(2) = '1') then
         next_state <= TEST4;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST4 =>
      next_state <= TEST4;
      if (KEY(7) = '1') then
         next_state <= TEST5;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST5 =>
      next_state <= TEST5;
      if (KEY(4) = '1') then
         next_state <= TEST6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST6 =>
      next_state <= TEST6;
      if (KEY(0) = '1') then
         next_state <= TEST_FIRST_7;
	   elsif (KEY(8)  = '1') then
			next_state <= TEST_SECOND_7;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_FIRST_7 =>
      next_state <= TEST_FIRST_7;
      if (KEY(7) = '1') then
         next_state <= TEST_FIRST_8;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_FIRST_8 =>
      next_state <= TEST_FIRST_8;
      if (KEY(3) = '1') then
         next_state <= TEST_FIRST_9;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_FIRST_9 =>
      next_state <= TEST_FIRST_9;
      if (KEY(4) = '1') then
         next_state <= TEST_FIRST_10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_FIRST_10 =>
      next_state <= TEST_FIRST_10;
      if (KEY(15) = '1') then
         next_state <= PRINT_OK_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_SECOND_7 =>
      next_state <= TEST_SECOND_7;
      if (KEY(5) = '1') then
         next_state <= TEST_SECOND_8;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_SECOND_8 =>
      next_state <= TEST_SECOND_8;
      if (KEY(1) = '1') then
         next_state <= TEST_SECOND_9;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_SECOND_9 =>
      next_state <= TEST_SECOND_9;
      if (KEY(7) = '1') then
         next_state <= TEST_SECOND_10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_SECOND_10 =>
      next_state <= TEST_SECOND_10;
      if (KEY(4) = '1') then
         next_state <= TEST_SECOND_11;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_SECOND_11 =>
      next_state <= TEST_SECOND_11;
		if (KEY(4) = '1') then
         next_state <= TEST_SECOND_12;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	 when TEST_SECOND_12 =>
      next_state <= TEST_SECOND_12;
      if (KEY(15) = '1') then
         next_state <= PRINT_OK_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when ERROR =>
      next_state <= ERROR;
      if (KEY(15) = '1') then
         next_state <= PRINT_ERROR_MESSAGE;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_OK_MESSAGE =>
      next_state <= PRINT_OK_MESSAGE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ERROR_MESSAGE =>
      next_state <= PRINT_ERROR_MESSAGE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ERROR_MESSAGE =>
      FSM_MX_MEM     <= '0';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_OK_MESSAGE =>
      FSM_MX_MEM     <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;