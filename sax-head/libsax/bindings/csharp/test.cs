using System;

public class SaXTest {
	static void Main() {
		Console.WriteLine("Hello World");

		using (SaXImport desktop = new SaXImport ( 5 )) // SaX.SAX_DESKTOP -> abort()
		using (SaXImport card    = new SaXImport ( 4 ))
		using (SaXImport path    = new SaXImport ( 9 ))
		using (SaXInit init = new SaXInit())
		{
			Console.WriteLine("Created SaXInit " + init);

			card.doImport();
			desktop.doImport();
			path.doImport();

			using ( SaXManipulateDesktop manip = new SaXManipulateDesktop (
				desktop,card,path,0
			))
			{
				String name = manip.getMonitorName();
				Console.WriteLine("Monitor name: " + name);

				//String[] lala = manip.getCDBMonitorList();
				//Console.WriteLine("First element: " + lala[0]);
			}
		}
	}
}
