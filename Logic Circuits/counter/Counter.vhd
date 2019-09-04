library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_unsigned.ALL;


entity Counter is

	port(
		clk :in std_logic;
		mode, reset : in std_logic;
		en1 , en2 : out std_logic;
		num : out std_logic_vector(6 downto 0)
		);
		
		
end Counter;

architecture Behavioral of Counter is

	signal cnt : std_logic_vector(25 downto 0);
	signal temp1 : std_logic_vector(3 downto 0);
	signal temp2 : std_logic_vector(3 downto 0);
	signal data_in : std_logic_vector(3 downto 0);
	
	component Seg port(
		data_in: in std_logic_vector(3 downto 0);
		data_out: out std_logic_vector(6 downto 0)
		);
	end component;

begin
	seg1 : Seg port map( data_in => data_in , data_out => num );
	process(clk)
	begin
		
		if (rising_edge(clk)) then
			cnt <= cnt+1 ;
			
			if ( cnt(10)='1') then
				en1 <= '1';
				en2 <= '0';
				data_in <= temp1;
			else
				en1 <= '0';
				en2 <= '1';
				data_in <= temp2;
			end if;
			
			if (reset = '1') then
				temp1 <= "0000";
				temp2 <= "0000";
			end if;
			
			if (cnt(25)='1') then
				cnt <= (others => '0');
				if (mode = '1') then
					temp1 <= temp1 + 1;
					
					if (temp1 = "1010") then
						temp1 <= "0000";
						temp2 <= temp2 + 1;
						if ( temp2 = "1010") then
							temp2 <= "0000";
						end if;
					end if;
					
				else
					temp1 <= temp1 - 1;
					
					if (temp1 = "1111") then
						temp1 <= "1001";
						temp2 <= temp2 - 1;
						if (temp2 = "1111") then
							temp2 <= "1001";
						end if;
					end if;
					
				end if;
			end if;
		
		end if;
	end process;
			

end Behavioral;

