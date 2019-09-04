----------------------------------------------------------------------------------
-- VGA DRIVER  
-- GOAL : WHITE SQUARE APPEARS ON TOP LEFT CORNER WITH A MARGIN
-- MOHAMMAD MAHDI HEYDARI
-- 9423045
-- LOGIC CIRCUITS 1396
----------------------------------------------------------------------------------

-- First we determine which libraries to use :
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;

-- in this part we determine inputs and outputs and their types :
entity vga_driver is
    Port ( CLK : in  STD_LOGIC; --initial clock
           RST : in  STD_LOGIC; --controlling input for reset 
           HSYNC : out  STD_LOGIC; --horizontal sync
           VSYNC : out  STD_LOGIC; --vertical sync
           RGB : out  STD_LOGIC_VECTOR (2 downto 0)); --the color with 8 varieties for drawing 
end vga_driver;


-- in architecture we determine the behavior
architecture Behavioral of vga_driver is

	signal clk25 : std_logic := '0'; -- a clock with half frequency of initial clock 
	
	constant HD : integer := 639; -- horizontal display
	constant HFP : integer := 16; -- horizontal front porch
	constant HSP : integer := 96; -- horizontal sync pulse
	constant HBP : integer := 48; -- horizontal back porch
	
	constant VD : integer := 479; -- vertical display
	constant VFP : integer := 10; -- vertical front porch
	constant VSP : integer := 2;  -- vertical sync pulse
	constant VBP : integer := 33; -- vertical back porch
	
	signal hPos : integer := 0; -- horizontal position
	signal vPos : integer := 0; -- vertical position
	
	signal videoOn : std_logic := '0'; -- shows if the picture is ready on screen
	
begin

-- process to create clk25 MHz
clk_div:process(CLK)
begin
	if(CLK'event and CLK = '1')then
	clk25 <= not clk25;
	end if;
end process;


-- process to calculate horizontal position
Horizontal_position_counter:process(clk25, RST)
begin
	if(RST = '1')then
	hPos <= 0;
	elsif(clk25'event and Clk25 = '1')then
		if(hPos = (HD + HFP + HSP + HBP))then
			hPos <= 0;
		else
			hPos <= hPos + 1;
		end if;
	end if;
end process;


-- process to calculate vertical position
-- we also need hPos hear to decide what to do
Vertical_position_counter:process(clk25, RST, hPos)
begin
	if(RST = '1')then
	vPos <= 0;
	elsif(clk25'event and Clk25 = '1')then
		if(hPos = (HD + HFP + HSP + HBP))then
			if(vPos = (VD + VFP + VSP + VBP))then
				vPos <= 0;
			else
				vPos <= vPos + 1;
			end if;	
		end if;
	end if;
end process;



-- process to calculate horizontal synchronisation
-- the following outputs HSYNC and VSYNC shows whether we are in available part or not
Horizontal_synchronisation:process(clk25, RST, hPos)
begin
	if(RST = '1')then
		HSYNC <= '0';
	elsif(clk25'event and clk25 = '1')then
		if((hPos <= (HD + HFP)) OR (hPos > HD + HFP + HSP))then
			HSYNC <= '1';
		else
			HSYNC <= '0';
		end if;
	end if;	
end process;



-- process to calculate vertical synchronisation
Vertical_synchronisation:process(clk25, RST, vPos)
begin
	if(RST = '1')then
		VSYNC <= '0';
	elsif(clk25'event and clk25 = '1')then
		if((vPos <= (VD + VFP)) OR (vPos > VD + VFP + VSP))then
			VSYNC <= '1';
		else
			VSYNC <= '0';
		end if;
	end if;	
end process;



-- process to get videoOn value
video_on:process(clk25, RST, hPos, vPos)
begin
	if(RST = '1')then
		videoOn <= '0';
	elsif(clk25'event and clk25 = '1')then
		if(hPos<=HD and vPos<=VD)then
			videoOn <= '1';
		else
			videoOn <= '0';
		end if;	
	end if;	
end process;


-- in the end we should draw something on the screen 
-- since it's up to us , we use the simplest shape possible and with just one color (white)
draw:process(clk25, RST, hPos, vPos, videoOn)
begin
	if(RST = '1')then
		RGB <= "000";
	elsif(clk25'event and clk25 = '1')then
		if(videoOn = '1')then
		if((hPos >= 10 and hPos <= 60) AND (vPos >= 10 and vPos <= 60))then
			RGB <= "111"; -- white
		else --otherwise we keep it black
			RGB <= "000";
		end if;
		else
			RGB <= "000";
		end if;	
	end if;	
end process;

end Behavioral;

-- The End