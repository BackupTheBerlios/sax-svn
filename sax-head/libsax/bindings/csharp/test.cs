using System;

public class SaXTest {
	static void Main() {
		Console.WriteLine("Hello World");

		using (SaXInit init = new SaXInit())
		{
			Console.WriteLine("Created SaXInit" + init);
		}
	}
}
