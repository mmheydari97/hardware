library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity Seg is

	port(
		data_in: in std_logic_vector(3 downto 0);
		data_out: out std_logic_vector(6 downto 0)
		);
		
end Seg;

architecture Behavioral of Seg is

begin
	process(data_in)
	begin
		
		case data_in is 
			when "0000" => data_out <= "0111111";
			when "0001" => data_out <= "0000110";
			when "0010" => data_out <= "1011011";
			when "0011" => data_out <= "1001111";
			when "0100" => data_out <= "1100110";
			when "0101" => data_out <= "1101101";
			when "0110" => data_out <= "1111101";
			when "0111" => data_out <= "0000111";
			when "1000" => data_out <= "1111111";
			when "1001" => data_out <= "1101111";
			when others => data_out <= "0000000";
		end case;
	end process;

end Behavioral;

