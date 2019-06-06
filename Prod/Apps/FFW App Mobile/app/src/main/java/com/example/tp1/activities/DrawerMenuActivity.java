package com.example.tp1.activities;

import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.ViewSwitcher;

import com.example.tp1.R;
import com.example.tp1.fragments.ForumFragment;
import com.example.tp1.fragments.NewsFragment;
import com.example.tp1.fragments.ProfilFragment;
import com.example.tp1.fragments.ServicesFragment;
import com.example.tp1.models.User;
import com.google.gson.Gson;

public class DrawerMenuActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private View disconnectItem;
    private User currentUser;
    private String userString;

    private TextView emailNavHeader;

    //FRAGMENTS = declared
    private Fragment fragmentProfil;
    private Fragment fragmentNews;
    private Fragment fragmentServices;
    private Fragment fragmentForum;

    //FOR DATAS - identify each fragments with a number
    private static final int FRAGMENT_PROFIL = 0;
    private static final int FRAGMENT_NEWS = 1;
    private static final int FRAGMENT_SERVICES = 2;
    private static final int FRAGMENT_FORUM = 3;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_drawer_menu);

        //create the object User
        SharedPreferences prefs = getSharedPreferences("authentication_dico",MODE_PRIVATE);
        userString = prefs.getString("user", null) ;
        currentUser = new Gson().fromJson(userString, User.class);

        //configure the toolbar to be the actionbar
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        //link the actionbar to the drawer menu
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
                                        //params : activity hosting the drawer, drawer layout to link, drawable (img), string to describe the "open" action for accessibility, string to describe the "close" action
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        //add the email to the header of the navigation drawer
        View headerView = navigationView.getHeaderView(0);
        emailNavHeader = (TextView)headerView.findViewById(R.id.navHeader_email);
        emailNavHeader.setText(currentUser.getEmail());

        this.showNewsFragment();

    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            alertDisconnect();
        }
    }


    private void alertDisconnect(){
        new AlertDialog.Builder(DrawerMenuActivity.this)
                .setTitle(getResources().getString(R.string.alert_title))
                .setMessage(getResources().getString(R.string.alert_disconnect_message))
                .setPositiveButton(getResources().getString(R.string.alert_yes), new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        deconnection(); //supprime les éléments de la mémoire locale du mobile
                        DrawerMenuActivity.super.onBackPressed();
                    }
                })
                .setNegativeButton(getResources().getString(R.string.alert_no), new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        //rien
                    }
                }).setCancelable(false).show();
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_profil) {
            this.showFragment(FRAGMENT_PROFIL);

        } else if (id == R.id.nav_forum) {
            this.showFragment(FRAGMENT_FORUM);

        } else if (id == R.id.nav_news) {
            this.showFragment(FRAGMENT_NEWS);

        } else if (id == R.id.nav_prem_services) {
            this.showFragment(FRAGMENT_SERVICES);

        } else if (id == R.id.nav_disconnect) {
            this.alertDisconnect();
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    private void showFragment(int fragmentIdentifier){
        switch (fragmentIdentifier){
            case FRAGMENT_NEWS :
                this.showNewsFragment();
                break;
            case FRAGMENT_PROFIL :
                this.showProfilFragment();
                break;
            case FRAGMENT_SERVICES:
                this.showServicesFragment();
                break;
            case FRAGMENT_FORUM :
                this.showForumFragment();
                break;
            default:
                break;
        }
    }

    private void showNewsFragment(){
        if(this.fragmentNews == null){
            this.fragmentNews = NewsFragment.newInstance();
        }
        getSupportActionBar().setTitle("News");
        this.startTransactionFragment(this.fragmentNews);
    }

    private void showProfilFragment(){
        if(this.fragmentProfil == null){
            this.fragmentProfil = ProfilFragment.newInstance();
        }
        getSupportActionBar().setTitle("Profil");
        this.startTransactionFragment(this.fragmentProfil);
    }

    private void showServicesFragment(){
        if(this.fragmentServices == null){
            this.fragmentServices = ServicesFragment.newInstance();
        }
        getSupportActionBar().setTitle("Services");
        this.startTransactionFragment(this.fragmentServices);
    }

    private void showForumFragment(){
        if(this.fragmentForum == null){
            this.fragmentForum = ForumFragment.newInstance();
        }
        getSupportActionBar().setTitle("Forum");
        this.startTransactionFragment(this.fragmentForum);
    }

    private void startTransactionFragment(Fragment fragment) {
        if(!fragment.isVisible()){
            Bundle userBundle = new Bundle();
            userBundle.putString("user",userString);
            fragment.setArguments(userBundle);
            getSupportFragmentManager().beginTransaction().replace(R.id.drawer_frame_layout, fragment).commit();
        }
    }

    private void deconnection(){
        SharedPreferences prefs = getSharedPreferences("authentication_dico", MODE_PRIVATE);
        SharedPreferences.Editor edit = prefs.edit();
        prefs.edit().remove("email").commit();
        prefs.edit().remove("mdp").commit();
    }

}
