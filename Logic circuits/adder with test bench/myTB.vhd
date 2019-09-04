--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   13:59:45 05/14/2017
-- Design Name:   
-- Module Name:   C:/Documents and Settings/Administrator/Desktop/TB_test/Adder/myTB.vhd
-- Project Name:  Adder
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: myAdder
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY myTB IS
END myTB;
 
ARCHITECTURE behavior OF myTB IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT myAdder
    PORT(
         a : IN  std_logic;
         b : IN  std_logic;
         c_in : IN  std_logic;
         s : OUT  std_logic;
         c_out : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal a : std_logic := '0';
   signal b : std_logic := '0';
   signal c_in : std_logic := '0';

 	--Outputs
   signal s : std_logic;
   signal c_out : std_logic;
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: myAdder PORT MAP (
          a => a,
          b => b,
          c_in => c_in,
          s => s,
          c_out => c_out
        );

   -- Stimulus process
   stim_proc: process
   begin		
	a<='0'; b<='0'; c_in<='0';
	wait for 10 ns;
	a<='0'; b<='0'; c_in<='1';
	wait for 10 ns;
	a<='0'; b<='1'; c_in<='0';
	wait for 10 ns;
	a<='0'; b<='1'; c_in<='1';
	wait for 10 ns;
	a<='1'; b<='0'; c_in<='0';
	wait for 10 ns;
	a<='1'; b<='0'; c_in<='1';
	wait for 10 ns;
	a<='1'; b<='1'; c_in<='0';
	wait for 10 ns;
	a<='1'; b<='1'; c_in<='1';
	wait for 10 ns;
   end process;

END;
