package com.example.tp1.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.ViewSwitcher;

import com.example.tp1.R;
import com.example.tp1.models.NewsPost;

import java.util.List;

public class NewsPostAdapter extends BaseAdapter {
    private List<NewsPost> posts;
    private Context context;

    public NewsPostAdapter(List<NewsPost> lfp, Context c){
        this.context = c;
        this.posts = lfp;
    }

    @Override
    public int getCount() {
        return this.posts.size();
    }

    @Override
    public Object getItem(int position) {
        return this.posts.get(position);
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {


        if(convertView == null){
            LayoutInflater inflater = LayoutInflater.from(this.context);
            convertView = inflater.inflate(R.layout.row_posts_news, null);
        }
        NewsPost currentPost = (NewsPost)getItem(position);

        TextView post_title_date = (TextView)convertView.findViewById(R.id.rpn_title_date);
        TextView post_content = (TextView)convertView.findViewById(R.id.rpn_content);
        TextView post_author = (TextView)convertView.findViewById(R.id.rpn_author);

        ImageButton favorite_icone_deselected = (ImageButton)convertView.findViewById(R.id.rpn_icone_favorite_deselected);
        ImageButton favorite_icone_selected = (ImageButton)convertView.findViewById(R.id.rpn_icone_favorite_selected);
        final ViewSwitcher favorite_switcher = (ViewSwitcher)convertView.findViewById(R.id.rpn_favorite_switcher);

        favorite_icone_deselected.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                favorite_switcher.showNext();
            }
        });

        favorite_icone_selected.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                favorite_switcher.showPrevious();
            }
        });

        ImageButton expand_less_icone = (ImageButton)convertView.findViewById(R.id.rpn_expand_less_comment_post);
        ImageButton expand_more_icone = (ImageButton)convertView.findViewById(R.id.rpn_expand_more_comment_post);
        final ViewSwitcher expand_switcher = (ViewSwitcher)convertView.findViewById(R.id.rpn_expand_comment_switcher);

        expand_less_icone.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                expand_switcher.showNext();
            }
        });

        expand_more_icone.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                expand_switcher.showPrevious();
            }
        });

        post_title_date.setText(currentPost.getTitle());
        post_content.setText(currentPost.getContent());
        post_author.setText(currentPost.getAuthor());

        return convertView;
    }
}
