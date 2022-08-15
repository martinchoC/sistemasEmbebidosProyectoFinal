import { Routes, RouterModule } from '@angular/router';
import { IndexComponent } from './index/index.component';
import { PagesComponent } from './pages.component';
import { SavesComponent } from './saves/saves.component';
import { SetupComponent } from './setup/setup.component';


const pagesRoutes: Routes = [
    {path: '' ,
    component: PagesComponent,
    children:[
                {path: 'index' ,component: IndexComponent},
                {path: 'setup' ,component: SetupComponent},
                {path: 'saves' ,component: SavesComponent},
                { path: '', redirectTo: '/index', pathMatch: 'full' }
               ]}
]

export const PAGES_ROUTES = RouterModule.forRoot(pagesRoutes,{useHash:true})
