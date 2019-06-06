package com.example.tp1.activities;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.example.tp1.R;


public class MainActivity extends AppCompatActivity {
    private Button connection_btn;
    private Button inscription_btn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        this.configureToolbar();

        connection_btn = (Button)findViewById(R.id.connection_btn);
        inscription_btn = (Button)findViewById(R.id.inscription_btn);


        inscription_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent inscriptionIntent = new Intent(MainActivity.this, InscriptionActivity.class );
                startActivity(inscriptionIntent);
            }
        });

        connection_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent connectionIntent = new Intent(MainActivity.this, ConnectionActivity.class);
                startActivity(connectionIntent);
            }
        });

        if(checkConnection()){
            Intent homeIntent = new Intent(MainActivity.this, DrawerMenuActivity.class);
            startActivity(homeIntent);
        }


    }


//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        getMenuInflater().inflate(R.menu.menu_main_activity, menu);
//        return true;
//    }

    private void configureToolbar(){
        Toolbar toolbar = (Toolbar)findViewById(R.id.activity_main_toolbar);
        setSupportActionBar(toolbar);
    }

    private boolean checkConnection(){
        final SharedPreferences prefs = getSharedPreferences("authentication_dico",MODE_PRIVATE);
        String email_value = prefs.getString("email", null);
        String mdp_value = prefs.getString("mdp", null);
        if(email_value != null && mdp_value != null){
            //connexion bdd/api pour vérifier la connexion et retourne true si connexion ok
            return true;
        }
        return false;
    }
}
