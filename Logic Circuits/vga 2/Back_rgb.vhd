library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Back_rgb is
	port(
		clk, reset : in std_logic;
		sw : in std_logic_vector(2 downto 0);
		hsync, vsync : out std_logic;
		rgb : out std_logic_vector(2 downto 0)
		);

end Back_rgb;

architecture arch of Back_rgb is

	signal rgb_reg : std_logic_vector(2 downto 0);
	signal video_on : std_logic;
	
	component vga_sync is
	port(
		clk, reset : in std_logic;
		hsync, vsync : out std_logic;
		video_on, p_tick : out std_logic;
		pixel_x, pixel_y : out std_logic_vector(9 downto 0)
		);
	end component;
	

begin

	vga : vga_sync port map (clk => clk, reset => reset, hsync => hsync, vsync => vsync, video_on => video_on, p_tick => open, pixel_x => open, pixel_y => open );
	
	process (clk, reset)
	begin
		if (reset = '1') then 
			rgb_reg <= ( others => '0');
		elsif (rising_edge(clk)) then
			rgb_reg <= sw ;
		end if;
	end process;
	
	rgb <= rgb_reg when video_on = '1' else "000";

end arch;

