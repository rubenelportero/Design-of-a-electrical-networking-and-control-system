package com.makingbytes.tfg_rubengarciasegovia;

import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;
import android.widget.Button;
import android.view.KeyEvent;
import android.app.AlertDialog;
import android.content.DialogInterface;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.net.URLConnection;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {
    public static String direccion = "Hola";
    public String contra = "";
    private ImageView imgImagen;
    public boolean actu = false;
    public boolean ftp = false;
    public boolean introducido = false;
    public String respuesta_url = "Hello World";
    public boolean corte = false;
    public int seccion = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();
        imgImagen = (ImageView) findViewById(R.id.imagen);
        imgImagen.setVisibility(View.GONE);
        ocultarcontrol();

        final Button button = findViewById(R.id.button_options);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                ocultar();
                EditText servidor = (EditText) findViewById(R.id.editText3);
                EditText contra_introducido = (EditText) findViewById(R.id.editText2);
                CargaImagenes nuevaTarea = new CargaImagenes();
                direccion = servidor.getText().toString();
                contra = contra_introducido.getText().toString();
                nuevaTarea.execute(direccion + "/grafica.php?type0=4");
                imgImagen.setRotation(90);
                imgImagen.getLayoutParams().height += 850;
                imgImagen.getLayoutParams().width += 2000;
                imgImagen.setVisibility(View.VISIBLE);
                introducido = true;
                actualizar_control();
                actualizar_fotos();
            }
        });

        final Button button_actuador = findViewById(R.id.button_actuador);

        button_actuador.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if(actu == false){
                    conectar_url(direccion + "/movil.php?pass="+contra+"&type=1&trigger=0");
                    button_actuador.setText("Actuador Activado");
                    actu = true;
                }else {
                    conectar_url(direccion + "/movil.php?pass="+contra+"&type=0&trigger=0");
                    button_actuador.setText("Actuador Desactivado");
                    actu = false;
                }
            }
        });
        final Button button_ftp = findViewById(R.id.button_ftp);

        button_ftp.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if(ftp == false){
                    conectar_url(direccion + "/movil.php?pass="+contra+"&type=1&trigger=1");
                    button_ftp.setText("FTP Activado");
                    ftp = true;
                }else {
                    conectar_url(direccion + "/movil.php?pass="+contra+"&type=0&trigger=1");
                    button_ftp.setText("FTP Desactivado");
                    ftp = false;
                }
            }
        });
        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

    }
    private void actualizar_control(){
        final Handler handler = new Handler();
        Timer timer = new Timer();
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                handler.post(new Runnable() {
                    public void run() {
                        actualizar_estado();
                    }
                });
            }
        };
        timer.schedule(task, 1,5000);
    }
    private void actualizar_fotos(){
        final Handler handler = new Handler();
        Timer timer = new Timer();
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                handler.post(new Runnable() {
                    public void run() {
                        switch(seccion){
                            case 1:
                                seleccionarfoto(direccion + "/grafica.php?type0=4");
                                break;

                            case 2:
                                seleccionarfoto(direccion + "/grafica.php?type0=0");
                                break;

                            case 3:
                                seleccionarfoto(direccion + "/grafica.php?type0=1");
                                break;

                            case 4:
                                seleccionarfoto(direccion + "/grafica.php?type0=3");
                                break;
                            case 5:
                                break;
                        }
                    }
                });
            }
        };
        timer.schedule(task, 1,60000);
    }
    private void ocultarcontrol() {
        TextView estado1 = (TextView) findViewById(R.id.control_estado_actual);
        TextView estado2 = (TextView) findViewById(R.id.Control_estado);
        Button botonactuador = (Button) findViewById(R.id.button_actuador);
        Button botonftp = (Button) findViewById(R.id.button_ftp);
        estado1.setVisibility(View.GONE);
        estado2.setVisibility(View.GONE);
        botonftp.setVisibility(View.GONE);
        botonactuador.setVisibility(View.GONE);
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
            // Con esto detectamos el botón de atrás
            AlertDialog.Builder alert = new AlertDialog.Builder(this);
            alert.setTitle("Salir");
            alert.setMessage("¿Quieres salir?");
            alert.setNegativeButton("No", null);
            alert.setPositiveButton("Si", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialogo1, int id) {
                    pirarse();
                }
            });
            alert.show();
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    private void pirarse() {
        Toast.makeText(getApplicationContext(), "...Cerrando aplicación...",
                Toast.LENGTH_SHORT).show();
        this.finish();
    }

    private void ocultar() {
        final Button button = findViewById(R.id.button_options);
        TextView textouno = (TextView) findViewById(R.id.pass_info);
        TextView textodos = (TextView) findViewById(R.id.textView2);
        EditText cajauno = (EditText) findViewById(R.id.editText3);
        EditText cajados = (EditText) findViewById(R.id.editText2);
        button.setVisibility(View.GONE);
        textouno.setVisibility(View.GONE);
        textodos.setVisibility(View.GONE);
        cajauno.setVisibility(View.GONE);
        cajados.setVisibility(View.GONE);
    }

    private void seleccionarfoto(String direccion) {
        CargaImagenes nuevaTarea = new CargaImagenes();
        nuevaTarea.execute(direccion);
        imgImagen.setVisibility(View.VISIBLE);
    }

    private void conectar_url(String direccion) {
        cargar_url nuevaTarea = new cargar_url();
        nuevaTarea.execute(direccion);
        try {
            Thread.sleep(100);
        } catch (InterruptedException ex) {
        }
        }
    private void actualizar_estado(){
        lectura_estado nuevaTarea = new lectura_estado();
        nuevaTarea.execute(direccion + "/movil.php?pass="+contra+"&type=2&trigger=2");
        lectura_boton nuevaTarea2 = new lectura_boton();
        nuevaTarea2.execute(direccion + "/movil.php?pass="+contra+"&type=2&trigger=0");
        lectura_ftp nuevaTarea3 = new lectura_ftp();
        nuevaTarea3.execute(direccion + "/movil.php?pass="+contra+"&type=2&trigger=1");
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.potencias && introducido == true) {
            ocultarcontrol();
            seleccionarfoto(direccion + "/grafica.php?type0=4");
            seccion= 1;
        } else if (id == R.id.medias && introducido == true) {
            ocultarcontrol();
            seleccionarfoto(direccion + "/grafica.php?type0=0");
            seccion= 2;
        } else if (id == R.id.frecuencias && introducido == true) {
            ocultarcontrol();
            seleccionarfoto(direccion + "/grafica.php?type0=1");
            seccion= 3;
        } else if (id == R.id.fdp && introducido == true) {
            ocultarcontrol();
            seleccionarfoto(direccion + "/grafica.php?type0=3");
            seccion= 4;
        } else if (id == R.id.control && introducido == true) {
            seccion= 5;
            imgImagen.setVisibility(View.GONE);
            TextView estado1 = (TextView) findViewById(R.id.control_estado_actual);
            TextView estado2 = (TextView) findViewById(R.id.Control_estado);
            Button botonactuador = (Button) findViewById(R.id.button_actuador);
            Button botonftp = (Button) findViewById(R.id.button_ftp);
            estado1.setVisibility(View.VISIBLE);
            estado2.setVisibility(View.VISIBLE);
            botonftp.setVisibility(View.VISIBLE);
            botonactuador.setVisibility(View.VISIBLE);
            actualizar_estado();
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    /*******************Lectura imagenes **************/
    private class CargaImagenes extends AsyncTask<String, Void, Bitmap> {

        ProgressDialog pDialog;

        @Override
        protected void onPreExecute() {
            // TODO Auto-generated method stub
            super.onPreExecute();

            pDialog = new ProgressDialog(MainActivity.this);
            pDialog.setMessage("Cargando Imagen");
            pDialog.setCancelable(true);
            pDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            pDialog.show();

        }

        @Override
        protected Bitmap doInBackground(String... params) {
            // TODO Auto-generated method stub
            Log.i("doInBackground", "Entra en doInBackground");
            String url = params[0];
            Bitmap imagen = descargarImagen(url);
            return imagen;
        }

        @Override
        protected void onPostExecute(Bitmap result) {
            // TODO Auto-generated method stub
            super.onPostExecute(result);
            imgImagen.setImageBitmap(result);
            pDialog.dismiss();
        }

        private Bitmap descargarImagen(String imageHttpAddress) {
            URL imageUrl = null;
            Bitmap imagen = null;
            try {
                imageUrl = new URL(imageHttpAddress);
                HttpURLConnection conn = (HttpURLConnection) imageUrl.openConnection();
                conn.connect();
                imagen = BitmapFactory.decodeStream(conn.getInputStream());
            } catch (IOException ex) {
                ex.printStackTrace();
            }

            return imagen;
        }

    }
    /***********   pasar datos por URL ***************************/
    private class cargar_url extends AsyncTask<String, Void, String> {

    ProgressDialog pDialog;

    @Override
    protected void onPreExecute() {
        // TODO Auto-generated method stub
        super.onPreExecute();

        pDialog = new ProgressDialog(MainActivity.this);
        pDialog.setMessage("Cargando...");
        pDialog.setCancelable(true);
        pDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        pDialog.show();

    }

    @Override
    protected String doInBackground(String... params) {
        // TODO Auto-generated method stub
        Log.i("doInBackground", "Entra en doInBackground");
        String url = params[0];
        String resultado = conectar(url);
        respuesta_url = resultado;
        return resultado;
    }

    @Override
    protected void onPostExecute(String result) {
        // TODO Auto-generated method stub
        super.onPostExecute(result);
        pDialog.dismiss();
    }

    private String conectar(String imageHttpAddress) {
        URL direUrl = null;
        String resulta = null;
        try {
            direUrl = new URL(imageHttpAddress);
            HttpURLConnection conn = (HttpURLConnection) direUrl.openConnection();
            conn.connect();
            InputStream stream = conn.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
            StringBuffer buffer = new StringBuffer();
            String line = "";
            line = reader.readLine();
            buffer.append(line);
            return buffer.toString();
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        return resulta;
    }

}
    /***********   Lectura del estado ***************************/
    private class lectura_estado extends AsyncTask<String, Void, String> {


        @Override
        protected void onPreExecute() {
            // TODO Auto-generated method stub
            super.onPreExecute();

        }

        @Override
        protected String doInBackground(String... params) {
            // TODO Auto-generated method stub
            Log.i("doInBackground", "Entra en doInBackground");
            String url = params[0];
            String resultado = conectar(url);
            return resultado;
        }

        @Override
        protected void onPostExecute(String result) {
            // TODO Auto-generated method stub
            super.onPostExecute(result);
            TextView estado = (TextView) findViewById(R.id.control_estado_actual);
            if (result.equals("1")){
                estado.setText("Corte");
                if(corte==false) {
                    AlertDialog.Builder alert = new AlertDialog.Builder(MainActivity.this);
                    alert.setTitle("Estado");
                    alert.setMessage("Corte en la alimentación");
                    alert.setNegativeButton("Ok", null);
                    alert.show();
                    corte = true;
                }
            } else {
                corte = false;
                estado.setText("Estable");
            }
        }

        private String conectar(String imageHttpAddress) {
            URL direUrl = null;
            String resulta = null;
            try {
                direUrl = new URL(imageHttpAddress);
                HttpURLConnection conn = (HttpURLConnection) direUrl.openConnection();
                conn.connect();
                InputStream stream = conn.getInputStream();
                BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
                StringBuffer buffer = new StringBuffer();
                String line = "";
                line = reader.readLine();
                buffer.append(line);
                return buffer.toString();
            } catch (IOException ex) {
                ex.printStackTrace();
            }

            return resulta;
        }

    }
    /***********   Lectura del actuador ***************************/
    private class lectura_boton extends AsyncTask<String, Void, String> {


        @Override
        protected void onPreExecute() {
            // TODO Auto-generated method stub
            super.onPreExecute();
        }

        @Override
        protected String doInBackground(String... params) {
            // TODO Auto-generated method stub
            Log.i("doInBackground", "Entra en doInBackground");
            String url = params[0];
            String resultado = conectar(url);
            return resultado;
        }

        @Override
        protected void onPostExecute(String result) {
            // TODO Auto-generated method stub
            super.onPostExecute(result);
            final Button button_actuador = findViewById(R.id.button_actuador);
            if (result.equals("1")){
                actu = true;
                button_actuador.setText("Actuador Activado");
            } else {
                actu = false;
                button_actuador.setText("Actuador Desactivado");
            }
        }

        private String conectar(String imageHttpAddress) {
            URL direUrl = null;
            String resulta = null;
            try {
                direUrl = new URL(imageHttpAddress);
                HttpURLConnection conn = (HttpURLConnection) direUrl.openConnection();
                conn.connect();
                InputStream stream = conn.getInputStream();
                BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
                StringBuffer buffer = new StringBuffer();
                String line = "";
                line = reader.readLine();
                buffer.append(line);
                return buffer.toString();
            } catch (IOException ex) {
                ex.printStackTrace();
            }

            return resulta;
        }

    }
    /***********   Lectura del ftp ***************************/
    private class lectura_ftp extends AsyncTask<String, Void, String> {


        @Override
        protected void onPreExecute() {
            // TODO Auto-generated method stub
            super.onPreExecute();

        }

        @Override
        protected String doInBackground(String... params) {
            // TODO Auto-generated method stub
            Log.i("doInBackground", "Entra en doInBackground");
            String url = params[0];
            String resultado = conectar(url);
            return resultado;
        }

        @Override
        protected void onPostExecute(String result) {
            // TODO Auto-generated method stub
            super.onPostExecute(result);
            final Button button_actuador = findViewById(R.id.button_ftp);
            if (result.equals("1")){
                ftp = true;
                button_actuador.setText("FTP Activado");
            } else {
                ftp = false;
                button_actuador.setText("FTP Desactivado");
            }
        }

        private String conectar(String imageHttpAddress) {
            URL direUrl = null;
            String resulta = null;
            try {
                direUrl = new URL(imageHttpAddress);
                HttpURLConnection conn = (HttpURLConnection) direUrl.openConnection();
                conn.connect();
                InputStream stream = conn.getInputStream();
                BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
                StringBuffer buffer = new StringBuffer();
                String line = "";
                line = reader.readLine();
                buffer.append(line);
                return buffer.toString();
            } catch (IOException ex) {
                ex.printStackTrace();
            }

            return resulta;
        }

    }
}